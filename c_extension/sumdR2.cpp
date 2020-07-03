#include <Python.h>

#include <stdlib.h>
#include <malloc.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <mutex>
#include <future>
#include <thread>
#include <map>

#define USE_THREAD 1
#define MAX_THREAD 10

#ifdef BUILD_PYMODULE
#define int_t     Py_ssize_t

typedef struct {
  PyObject_HEAD
  void *buffer;          /* in column-major-mode array of type 'id' */
  int nrows, ncols;      /* number of rows and columns */
  int   id;              /* DOUBLE, INT, COMPLEX */
  int_t shape[2];
  int_t strides[2];
  int_t ob_exports;
} matrix;

#define MAT_BUFD(O)  ((double *)((matrix *)O)->buffer)
#else
#include "cvxopt.h"
extern void dump_matrix(const matrix *m);
#endif

extern "C" 
{
    extern void dcopy_(int *n, double *x, int *incx, double *y, int *incy);
    extern void daxpy_(int *n, double *alpha, double *x, int *incx, double *y, int *incy);
}

static matrix * Matrix_New_Internal(int nrows, int ncols)
{
    matrix *m = new matrix;
    if (!m) {
        return NULL;
    }
    m->nrows = nrows;
    m->ncols = ncols;
    m->buffer = memalign(64, nrows * ncols * sizeof(double));
    if (!m->buffer) {
        delete m;
        m = NULL;
        return NULL;
    }
    return m;
}

static void Matrix_Delete_Internal(matrix *m)
{
    if (m && m->buffer) {
        free(m->buffer);
        m->buffer = NULL;
    }
    if (m) {
        delete m;
    }
}

extern "C" void pairwise_diff(const matrix *x, matrix *y, int N)
{
    int k = 0;
    int rx = x->nrows;
    int ry = y->nrows;

    double alpha = -1.0f;
    int n = y->nrows, ix = 1, iy = 1;
    for (int i = 0; i < N; i ++) {
        for (int j = i; j < N; j ++, k ++) {
            dcopy_(&n, MAT_BUFD(x) + i * rx, &ix, MAT_BUFD(y) + k * ry, &iy);
            if (j > i) {
                daxpy_(&n, &alpha, MAT_BUFD(x) + j * rx, &ix, MAT_BUFD(y) + k * ry, &iy);
            }
        }
    }
}

extern "C" void matrix_square_add(matrix *dst, const matrix *src)
{
    for (int i=0; i < dst->nrows; i++) {
        for (int j=0; j < dst->ncols; j++) {
            int pos = i * dst->ncols + j;
            double src_v = ((double*)src->buffer)[pos];
            ((double*)dst->buffer)[pos] += src_v * src_v;
        }
    }
}

extern "C" void matrix_transpose_2(matrix *src, matrix *dst)
{
    double *src_p = (double*)src->buffer;
    double *dst_p = (double*)dst->buffer;
    for (int i = 0; i < dst->nrows; i++) {
        for (int j = 0; j < dst->ncols; j++) {
            dst_p[i + j * dst->nrows] = src_p[i * src->nrows + j];
        }
    }
}

static std::mutex ddR2_lock;
static void do_sub_sumdR2(const std::vector<matrix*> &Ris, matrix *ddR2, int K)
{
    matrix *dR = Matrix_New_Internal(K, K * (K + 1) / 2);
    matrix *dRT = Matrix_New_Internal(K * (K + 1) / 2, K);
    matrix *ddR = Matrix_New_Internal(K * (K + 1) / 2, K * (K + 1) / 2);
    matrix *ddR2_t = Matrix_New_Internal(K * (K + 1) / 2, K * (K + 1) / 2);
    for (int i = 0; i < ddR2_t->ncols * ddR2_t->nrows; i ++) {
        ((double*)ddR2_t->buffer)[i] = .0f;
    }
    for (size_t i = 0; i < Ris.size() ; i ++) {
        // printf("i = %d\n", (int)i);
        pairwise_diff(Ris[i], dR, K);
        matrix_transpose_2(dR, dRT);
        pairwise_diff(dRT, ddR, K);
        matrix_square_add(ddR2_t, ddR);
    }
    ddR2_lock.lock();
    for (int i = 0; i < ddR2_t->ncols * ddR2_t->nrows; i ++) {
        ((double*)ddR2->buffer)[i] += ((double*)ddR2_t->buffer)[i];
    }
    ddR2_lock.unlock();
    Matrix_Delete_Internal(dR);
    Matrix_Delete_Internal(dRT);
    Matrix_Delete_Internal(ddR);
    Matrix_Delete_Internal(ddR2_t);
}

extern "C" void do_sumdR2(const std::vector<matrix*> &Ris, matrix *ddR2, int K)
{
#if USE_THREAD
    std::vector<std::vector<matrix*>> Ris_group;
    for (int i = 0; i < K; i ++) {
        if (Ris_group.rbegin() == Ris_group.rend() || Ris_group.rbegin()->size() == MAX_THREAD) {
            Ris_group.push_back(std::vector<matrix*>());
        }
        Ris_group.rbegin()->push_back(Ris[i]);
    }
    std::vector<std::thread> f;
    for (size_t i = 0; i < Ris_group.size(); i ++) {
        f.push_back(std::thread(do_sub_sumdR2, Ris_group[i], ddR2, K));
    }
    for (auto &it : f) {
        it.join();
    }
#else
    do_sub_sumdR2(Ris, ddR2, K);
#endif
}

extern "C" PyObject* sumdR2_C(PyObject *self, PyObject *args) {
    int K = 0;
    PyObject *Ris_object = NULL;
    std::vector<matrix*> Ris;
    matrix* ddR2 = NULL;

    if (!PyArg_ParseTuple(args, "OOi", &Ris_object, &ddR2, &K)) {
        return NULL;
    }
    if (!Ris_object || !PyList_Check(Ris_object)) {
        return NULL;
    }
    size_t Ris_size = PyList_Size(Ris_object);
    for (size_t i = 0; i < Ris_size; i ++) {
        matrix *m = (matrix*)PyList_GetItem(Ris_object, i);
        Ris.push_back(m);
    }
    do_sumdR2(Ris, ddR2, K);
    return Py_BuildValue("");
}

#ifdef BUILD_PYMODULE
static PyMethodDef sumdR2Methods[] = {
    {
        "sumdR2_C",
        sumdR2_C,
        METH_VARARGS,
        "Diffnet extension.\n\n"
    },
    {NULL, NULL, 0, NULL},  // sentinel
};

#if PY_MAJOR_VERSION >= 3
static PyModuleDef sumdR2_module = {
    PyModuleDef_HEAD_INIT,
    "sumdR2_C",
    "DiffNet Python C extension module.",
    -1,
    sumdR2Methods,
};

PyMODINIT_FUNC PyInit_spam() {
    PyObject* module;

    module = PyModule_Create(&sumdR2_module);
    if (module == NULL) {
        return NULL;
    }
    return module;
}
#else
PyMODINIT_FUNC initsumdR2_C() {
    PyObject* module;
    std::cout << "init module: init sumdR2_C" << std::endl;
    module = Py_InitModule3(
        "sumdR2_C", sumdR2Methods, "DiffNet Python C extension module.");
    if (module == NULL) {
        return;
    }
}
#endif

#endif

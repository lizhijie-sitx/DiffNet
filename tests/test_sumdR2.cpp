#include <limits>
#include "gtest/gtest.h"

#include "cvxopt.h"
#include "matrix_data.h"
#include "pairwise_diff1.h"
#include "pairwise_diff2.h"
#include <sstream>
#include <vector>

extern "C"
{
extern PyObject *c_api_object;
extern void initsumdR2(void);
extern void do_sumdR2(const std::vector<matrix*> &Ris, matrix *ddR2, int K);
extern void pairwise_diff(const matrix *x, matrix *y, int N);
extern void matrix_transpose_2(matrix *src, matrix *dst);
extern void matrix_square_add(matrix *dst, const matrix *src);
}

typedef matrix* (*Matrix_New_F)(int, int, int);

Matrix_New_F new_f = NULL;

//dump matrix
void dump_matrix(const matrix *m)
{
    std::vector<std::ostringstream> osss(m->nrows);
    for (int i = 0; i < m->nrows * m->ncols; i ++) {
        osss[i % m->nrows] << ((double*)m->buffer)[i] << " ";
    }
    std::cout << "++++++++++++++" << std::endl;

    for (auto &o : osss) {
        std::cout << o.str() << std::endl;
    }
    std::cout << "++++++++++++++" << std::endl;
}

TEST(test_sumdR2, matrix_transposse)
{
    const int ROWS = 2;
    const int COLS = 4;
    double src_data[ROWS][COLS] = {1,2,3,4,5,6,7,8};
    double dst_data[COLS][ROWS] = {1,3,5,7,2,4,6,8};
    matrix *src = new_f(ROWS, COLS, DOUBLE);
    memcpy(src->buffer, src_data, ROWS * COLS * sizeof(double));
    matrix *dst = new_f(COLS, ROWS, DOUBLE);
    matrix_transpose_2(src, dst);
    // std::cout << "transpose:" << std::endl;
    // std::cout << "src:" << std::endl;
    // dump_matrix(src);
    // std::cout << "dst:" << std::endl;
    // dump_matrix(dst);
    for (int i = 0; i < ROWS * COLS; i ++) {
        double dst_v = ((double*)dst->buffer)[i];
        double dst_c_v = ((double*)dst_data)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

TEST(test_sumdR2, matrix_transposse2)
{
    const int ROWS = 4;
    const int COLS = 4;
    double src_data[ROWS][COLS] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    double dst_data[COLS][ROWS] = {1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16};
    matrix *src = new_f(ROWS, COLS, DOUBLE);
    memcpy(src->buffer, src_data, ROWS * COLS * sizeof(double));
    matrix *dst = new_f(COLS, ROWS, DOUBLE);
    matrix_transpose_2(src, dst);
    for (int i = 0; i < ROWS * COLS; i ++) {
        double dst_v = ((double*)dst->buffer)[i];
        double dst_c_v = ((double*)dst_data)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

TEST(test_sumdR2, square_add)
{
    #define ROWS 2
    #define COLS 4
    double src_data[ROWS][COLS] = {{1,2,3,4},{5,6,7,8}};
    double dst_data[ROWS][COLS] = {{2,8,18,32},{50,72,98,128}};
    matrix *src = new_f(2, 4, DOUBLE);
    memcpy(src->buffer, src_data, ROWS * COLS * sizeof(double));
    matrix *dst = new_f(2, 4, DOUBLE);
    matrix_square_add(dst, src);
    matrix_square_add(dst, src);
    for (int i = 0; i < ROWS * COLS; i ++) {
        double dst_v = ((double*)dst->buffer)[i];
        double dst_c_v = ((double*)dst_data)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

TEST(test_sumdR2, pairwise_diff1)
{
    std::cout << "start test pairwise diff" << std::endl;
    int K = sizeof(Ris) / sizeof(Ris[0]);
    matrix *src = new_f(K + 1, K + 1, DOUBLE);
    matrix *dst = new_f(K, K*(K+1)/2, DOUBLE);
    matrix *dst_compare = new_f(K, K*(K+1)/2, DOUBLE);
    memcpy(src->buffer, pairwise_diff_src_1, src->ncols * src->nrows * sizeof(double));
    memcpy(dst_compare->buffer, pairwise_diff_dst_1, dst_compare->ncols * dst_compare->nrows * sizeof(double));
    pairwise_diff(src, dst, K);
    // std::cout << "pairwise dest matrix:" << std::endl;
    // dump_matrix(dst);
    // std::cout << "pairwise compare matrix:" << std::endl;
    // dump_matrix(dst_compare);
    for (int i = 0; i < dst->ncols * dst->nrows; i ++) {
        double dst_v = ((double*)dst->buffer)[i];
        double dst_c_v = ((double*)dst_compare->buffer)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

TEST(test_sumdR2, pairwise_diff2)
{
    std::cout << "start test pairwise diff2" << std::endl;
    int K = sizeof(Ris) / sizeof(Ris[0]);
    matrix *srcT = new_f(K*(K + 1)/2, K, DOUBLE);
    matrix *dst = new_f(K*(K+1)/2, K*(K+1)/2, DOUBLE);
    matrix *dst_compare = new_f(K*(K+1)/2, K*(K+1)/2, DOUBLE);
    memcpy(srcT->buffer, pairwise_diff_src_2, srcT->ncols * srcT->nrows * sizeof(double));
    memcpy(dst_compare->buffer, pairwise_diff_dst_2, dst_compare->ncols * dst_compare->nrows * sizeof(double));
    pairwise_diff(srcT, dst, K);
    // std::cout << "pairwise dest matrix:" << std::endl;
    // dump_matrix(dst);
    // std::cout << "pairwise compare matrix:" << std::endl;
    // dump_matrix(dst_compare);
    for (int i = 0; i < dst->ncols * dst->nrows; i ++) {
        double dst_v = ((double*)dst->buffer)[i];
        double dst_c_v = ((double*)dst_compare->buffer)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

TEST(test_sumdR2, do_sumdR2)
{
    std::cout << "start test sumdR2" << std::endl;
    int K = sizeof(Ris) / sizeof(Ris[0]);
    std::vector<matrix*> Ris_v;
    for (int i = 0; i < K; i ++) {
        matrix *m = new_f(K + 1, K + 1, DOUBLE);
        memcpy(m->buffer, &Ris[i][0], m->ncols * m->nrows * sizeof(double));
        Ris_v.push_back(m);
    }
    matrix *ddR2_v = new_f(K * (K + 1) / 2, K * (K + 1) / 2, DOUBLE);
    matrix *ddR2_compare = new_f(K * (K + 1) / 2, K * (K + 1) / 2, DOUBLE);
    memcpy(ddR2_compare->buffer, ddR2, ddR2_compare->ncols * ddR2_compare->nrows * sizeof(double));
    // dump_matrix(Ris_v[0]);
    // dump_matrix(Ris_v[1]);
    // dump_matrix(Ris_v[2]);
    // dump_matrix(Ris_v[3]);
    do_sumdR2(Ris_v, ddR2_v, K);
    // std::cout << "ddR2_v dest matrix:" << std::endl;
    // dump_matrix(ddR2_v);
    // std::cout << "ddR2_compare compare matrix:" << std::endl;
    // dump_matrix(ddR2_compare);
    for (int i = 0; i < ddR2_v->ncols * ddR2_v->nrows; i ++) {
        double dst_v = ((double*)ddR2_v->buffer)[i];
        double dst_c_v = ((double*)ddR2_compare->buffer)[i];
        double diff = dst_v - dst_c_v;
        EXPECT_LT(diff, 0.0000001);
    }
}

int main(int argc, char ** argv)
{
    static_assert(std::numeric_limits<double>::is_iec559,"IEEE 754 required");

    testing::InitGoogleTest(&argc,argv);

    Py_Initialize();
    initsumdR2();
    void ** api_base = (void**)PyCObject_AsVoidPtr(c_api_object);
    new_f = (Matrix_New_F)(api_base[0]);
    return RUN_ALL_TESTS();
}
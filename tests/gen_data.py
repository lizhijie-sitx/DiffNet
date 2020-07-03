from A_opt import sumdR2, pairwise_diff
from cvxopt import matrix, blas
import random
import numpy as np

def format_c_matrix(m, indent = 4, start_indent = 0):
    mat = m.T
    indent_s = ' ' * start_indent
    s = indent_s + '{\n'
    nrows = mat.size[0]
    ncols = mat.size[1]
    for i in range(nrows):
        column_data = []
        for j in range(ncols):
            column_data.append(str(mat[i,j]))
        s += (indent_s + ' ' * indent + ', '.join(column_data))
        if i == nrows - 1:
            s += '\n'
        else:
            s += ',\n'
    s += (indent_s + '}')
    return s

def gen_sumdR2_data(Ris, ddR2):
    with open("matrix_data.h", 'w') as f:
        size = Ris[0].size
        f.write("const double Ris[%d][%d] = {\n" % (len(Ris), size[0] * size[1]))
        for i in range(len(Ris)):
            s = format_c_matrix(Ris[i], 4, 4)
            f.write(s)
            if i == len(Ris) - 1:
                f.write('\n')
            else:
                f.write(',\n')
        f.write("};\n\n")

        size = ddR2.size
        f.write("const double ddR2[%d] = " % (size[0] * size[1]))
        s = format_c_matrix(ddR2, 4, 0)
        f.write(s + ';\n')

def gen_pairwise_diff_data(src, dst, name, index):
    with open(name, 'w') as f:
        size = src.size
        f.write("const double pairwise_diff_src_%d[%d] = " % (index, size[0] * size[1]))
        s = format_c_matrix(src, 4, 0)
        f.write(s + ';\n')

        size = dst.size
        f.write("const double pairwise_diff_dst_%d[%d] = " % (index, size[0] * size[1]))
        s = format_c_matrix(dst, 4, 0)
        f.write(s + ';\n')

def gen_data():
    K = 4
    random.seed(0)
    Ris = [ matrix(0.0, (K+1, K+1)) for i in xrange(K) ]
    for i in xrange(K):
        m = matrix(np.random.random((K+1, K+1)))
        blas.gemm(m, m, Ris[i], transB = 'T')
    ddR2 = sumdR2(Ris, K)
    gen_sumdR2_data(Ris, ddR2)

    diff_src = matrix( 0., (K + 1, K + 1))
    counter = 1
    for i in range(diff_src.size[0]):
        for j in range(diff_src.size[1]):
            diff_src[i,j] = counter
            counter += 1
    dR = matrix( 0., (K, K*(K+1)/2))
    pairwise_diff( diff_src[:K,:K], dR, K)
    gen_pairwise_diff_data(diff_src, dR, "pairwise_diff1.h", 1)

    diff_src = matrix(0., (K, K * (K + 1) / 2))
    counter = 1
    for i in range(diff_src.size[0]):
        for j in range(diff_src.size[1]):
            diff_src[i,j] = counter
            counter += 1
    dR = matrix( 0., (K*(K+1)/2, K*(K+1)/2))
    diff_srcT = diff_src.T
    pairwise_diff(diff_srcT, dR, K)
    gen_pairwise_diff_data(diff_srcT, dR, "pairwise_diff2.h", 2)
if __name__ == '__main__':
    gen_data()

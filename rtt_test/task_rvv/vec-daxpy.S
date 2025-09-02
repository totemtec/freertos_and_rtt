    .text
    .balign 4
    .global vec_daxpy
# void
# vec_daxpy(size_t n, const double a, const double *x, const double *y, double* z)
# {
#   size_t i;
#   for (i=0; i<n; i++)
#     z[i] = a * x[i] + y[i];
# }
#
# register arguments:
#     a0      n
#     a1      a
#     a2      x
#     a3      y
#     a4      z
vec_daxpy:
    vsetvli a5, a0, e8, m1, ta, ma
    vle16.v v0, (a2)
    sub a0, a0, a5
    add a2, a2, a5
    vle16.v v8, (a3)
    vmacc.vx v8, a1, v0
    vse16.v v8, (a4)
    add a3, a3, a5
    add a4, a4, a5
    bnez a0, vec_daxpy
    ret

    .text
    .balign 4
    .global vec_vadd
# void
# vec_daxpy(size_t n, const double *x, const double *y, double* z)
# {
#   size_t i;
#   for (i=0; i<n; i++)
#     z[i] = x[i] + y[i];
# }
#
# register arguments:
#     a0      n
#     a1      x
#     a2      y
#     a3      z
vec_vadd:
    vsetvli a4, a0, e16, m1, ta, ma
    vle16.v v0, (a1)
    sub a0, a0, a4
    slli a4, a4, 1
    add a1, a1, a4
    vle16.v v8, (a2)
    vadd.vv v8, v0, v8   #vadd.vv vd, vs2, vs1, vm # Vector-vector
    vse16.v v8, (a3)
    add a2, a2, a4
    add a3, a3, a4
    bnez a0, vec_vadd
    ret

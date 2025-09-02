void vec_vadd(size_t n, const data_t *x, const data_t *y, data_t* z);
n : element number
*x : source1  pointer address
*y : source2  pointer address
*z : destination pointer address
funtion : 
```
  for (i=0; i<n; i++)
  z[i] = x[i] + y[i];
```

void vec_vmul(size_t n, const data_t *x, const data_t *y, data_t* z);
n : element number;
*x : source1  pointer address
*y : source2  pointer address
*z : destination pointer address
funtion : 
```
  for (i=0; i<n; i++)
  z[i] = x[i] * y[i];
```

void vec_daxpy(size_t n, const data_t a, const data_t *x, const data_t *y, data_t* z);
n : element number;
a : constant 
*x : source1  pointer address
*y : source2  pointer address
*z : destination pointer address
funtion : 
```
  for (i=0; i<n; i++)
  z[i] = a * x[i] + y[i];
```

static int verify_int16(int n, const volatile  int16_t* test, const int16_t* verify)
n : compair data size
*test : computation results pointer address
*verify : correct results pointer address
funtion : 
```
if pass, return 0;
else if fault, return error location numbers
```

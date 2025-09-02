// See LICENSE for license details.

//**************************************************************************
// Memcpy benchmark
//--------------------------------------------------------------------------
//
// This benchmark tests a vectorized sazpy implementation.
// The input data (and reference data) should be generated using
// the daxpy_gendata.pl perl script and dumped to a file named
// dataset1.h.

#include "my_typedef.h"

#include <string.h>


static int verify_int16(int n, const volatile  int16* test, const int16* verify)
{
  int i;
  // Unrolled for faster verification
  for (i = 0; i < n/2*2; i+=2)
  {
    short int t0 = test[i], t1 = test[i+1];
    short int v0 = verify[i], v1 = verify[i+1];
    if (t0 != v0) return i+1;
    if (t1 != v1) return i+2;
  }
  if (n % 2 != 0 && test[n-1] != verify[n-1])
    return n;
  return 0;
}

//--------------------------------------------------------------------------
// Input/Reference Data

typedef int16 data_t;
#include "dataset1.h"

//--------------------------------------------------------------------------
// Main

void vec_vadd(size_t n, const data_t *x, const data_t *y, data_t* z);
void vec_vmul(size_t n, const data_t *x, const data_t *y, data_t* z);
void vec_daxpy(size_t n, const data_t a, const data_t *x, const data_t *y, data_t* z);

#include "rtthread.h"

static struct rt_mutex static_mutex;


void rvv_init(void)
{
	rt_err_t result;
    result = rt_mutex_init(&static_mutex, "smutex", RT_IPC_FLAG_FIFO);
}

void mux_get(void)
{
	rt_err_t rt_result = rt_mutex_take(&static_mutex, RT_WAITING_FOREVER);
}
void mux_release(void)
{
	rt_mutex_release(&static_mutex);
}
int rvv_main_add(void)
{
  rt_err_t rt_result;
  data_t results_data[DATA_SIZE];

  mux_get();
  vec_vadd(DATA_SIZE, input1_data, input2_data, results_data);
  mux_release();

  // Check the results
  int result;
  result = verify_int16( DATA_SIZE, results_data, verify_data );

  return result;
}

int rvv_main_mul(void)
{
  rt_err_t rt_result;
  data_t results_data[DATA_SIZE];
  int result = 0;

  mux_get();
  vec_vmul(DATA_SIZE, input1_data, input2_data, results_data);
  mux_release();

  for( int i = 0; i < DATA_SIZE; ++i ) {
	  int16 r = input1_data[i] * input2_data[i];
	  if( r != results_data[i] ) {
		  result += 1;
	  }
  }
  return result;
}



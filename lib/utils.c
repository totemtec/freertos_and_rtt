#include <stdint.h>
#include <string.h>
#include "../include/utils.h"

#include "my_typedef.h"
#include "uart.h"

uint64_t get_cycle_value()
{
    uint64_t cycle;

    cycle = read_csr(cycle);
    cycle += (uint64_t)(read_csr(cycleh)) << 32;

    return cycle;
}

void busy_wait(uint32_t us)
{
    uint64_t tmp;
    uint32_t count;

    count = us * CPU_FREQ_MHZ;
    tmp = get_cycle_value();

    while (get_cycle_value() < (tmp + count));
}

//note: the result is 0x2000 ????
void fpu_enable() {
  asm volatile("li a0, 0x00006000 & (0x00006000 >> 1)");
  asm volatile("csrs mstatus, a0");
  asm volatile("csrwi fcsr, 0");
}
//return ok
void test_pass(void)
{   
    asm volatile(
        "li  x3  , 1\n"
        "li  x10 , 0\n"
        "ecall\n"
    );
}
//return fail
void test_fail(void)
{   
    asm volatile(
        "li  x3  , 0\n"
        "li  x10 , 0\n"
        "ecall\n"      
    );
}
volatile int mttt;
void test_delay( int n )
{
    for(int i = 0;i < n; i++) {
        for(int i = 0;i < 100; i++) {
            mttt += 1;
        }
    }
}
void delay_ms( int n )
{
    for(int i = 0;i < n; i++) {
        test_delay( 1000 );
    }
}

void catch_err(void)
{
    while( 1 ) {
        ee_printf("ERR\n" );
    }
}

uint32 check_iter( uint32 in, int d )
{
    in += d;
    if( (in & 0x80000000) == 0) {
        return in << 1;
    }
    else {
        return (in << 1) | 1;
    }
}

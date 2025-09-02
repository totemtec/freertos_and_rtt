#include "encoding.h"
#include <stdint.h>

#include "my_typedef.h"
#include "global.h"

#define U32         *(volatile unsigned int *)

#if IS_VCS_NOT_SPIKE

	//@pz change the clint addr -> 0x00
	#define CoreGen_CLINT_BASE   0x00060000

	#define CoreGen_MSIP_ADDR   0x10
	#define CoreGen_MTIME_ADDR_L   0
	#define CoreGen_MTIME_ADDR_H   4
	#define CoreGen_MTIME_CMP_ADDR_L   8
	#define CoreGen_MTIME_CMP_ADDR_H   12
	#define MY_TIME 2000     //2000 limit test  20000 is ahbl   2000 is core
#else
	#define CoreGen_CLINT_BASE     0x02000000
	#define CoreGen_MSIP_ADDR   0x00
	#define CoreGen_MTIME_ADDR_L   0xbff8
	#define CoreGen_MTIME_ADDR_H   0xbffc
	#define CoreGen_MTIME_CMP_ADDR_L   0x4000
	#define CoreGen_MTIME_CMP_ADDR_H   0x4004

	//231: all save    225:gcc save fast
	#define MY_TIME 220
#endif



typedef unsigned long long   uint64_t;
void handle_trap()                            ;//注册中断函数
void clint_csr_cfg()                          ;//中断初始化
void CLINT_mtime_write(uint64_t mtime)        ;
void CLINT_mtime_cmp_write(uint64_t mtime_cmp);
uint64_t CLINT_mtime_cmp_read()               ;
void handle_clint()                           ;
/*********************************************/
void handle_trap_plic();//注册中断函数
void csr_cfg();//中断初始化
void PLIC_init_cfg(unsigned int num);//把中断优先级，使能设置为0
void PLIC_threshold_write(unsigned int context, unsigned int threshold);//阀值
void PLIC_id_clr(unsigned int id);//清除中断ID，当写入complete寄存器则表示中断完成
void PLIC_sourceX_cfg(unsigned int context, unsigned int num, unsigned int priority, unsigned int enable);//设置某个id的优先级和使能的值
unsigned int PLIC_id_read(unsigned int context);//读取id，表示仲裁成功，需要读取id进入中断函数
unsigned int PLIC_read_pending();//读取pending
void CLINT_msip_write(unsigned int misp_num);
void timer_init(void);

void simple_exc_handler();
// handle_trap function
extern void trap_entry(void);
extern void trap_end(void);
unsigned int get_mcause();

#include "rtthread.h"

int timer_num1;


void test_pass(void);
//--------------------------------------------------------------------------
void handle_trap_timer()
{    
    unsigned int csr_tmp;
    //mie.MEIE
    unsigned int csr_mstaus;
    unsigned int csr_mip;

    timer_num1 = timer_num1 + 1;


    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();

	//sw_setpend();


    //putchar( '*' );
    //putchar( '\n' );

    //csr_tmp = read_csr(mie);
    // write_csr(mie,(csr_tmp | 0xFFFF0F0F));//

    //CLINT_mtime_write(0);
    CLINT_mtime_cmp_write(MY_TIME);

    // clint_csr_cfg();
    //csr_mstaus = read_csr(mstatus);
    //csr_mip    = read_csr(mip);
    //ee_printf( "mip: %d\n",csr_mip);
    //ee_printf( "mstaus: %d\n",csr_mstaus);
}

void handle_trap_software()
{
    unsigned int csr_tmp;
    csr_tmp = read_csr(mie);
    write_csr(mie,(csr_tmp | 0xFFFF0FF0));
    CLINT_msip_write(0);

    //putchar( '#' );
}
//--------------------------------------------------------------------------
// CSR interrupt configuration function

void clint_csr_cfg()
{
    unsigned int csr_tmp;

    //mie.MEIE
    csr_tmp = read_csr(mie);
   
    //write_csr(mie,0x0);
    write_csr(mie,(csr_tmp | 0xFFFF0888));

    //mstatus.MIE
    csr_tmp = read_csr(mstatus);

    //write_csr(mstatus,0x0);
    write_csr(mstatus,(csr_tmp | 0x8));
}
void clint_disable()
{
	write_csr(mie, 0 );
}

/********************************************/
//mtime write and read 
uint64_t CLINT_mtime_read()
{
    uint64_t LOW, HIGH;
    uint64_t mtime_read;
    LOW = U32(CoreGen_CLINT_BASE + CoreGen_MTIME_ADDR_L);
    HIGH = U32(CoreGen_CLINT_BASE + CoreGen_MTIME_ADDR_H); 
    mtime_read = (HIGH << 32) | LOW;
    return mtime_read;
}

void CLINT_mtime_write_tmp(uint64_t mtime_num)
{
    unsigned int LOW, HIGH;
    LOW  = mtime_num;
    HIGH = mtime_num >> 32;
    U32(CoreGen_CLINT_BASE + CoreGen_MTIME_ADDR_L) = LOW  ;
    U32(CoreGen_CLINT_BASE + CoreGen_MTIME_ADDR_H) = HIGH ;
}
/**********************************************/
//software write
void CLINT_msip_write(unsigned int misp_num)
{
    U32(CoreGen_CLINT_BASE + CoreGen_MSIP_ADDR) = misp_num;

	//U32(CoreGen_CLINT_BASE + 0) = misp_num;
    //asm volatile ("csrw mip, 0");
}
/*********************************************/
//mtime write and read
uint64_t CLINT_mtime_cmp_read()
{
    uint64_t LOW, HIGH;
    uint64_t mtime_cmp_read;
    LOW = U32(CoreGen_CLINT_BASE + CoreGen_MTIME_CMP_ADDR_L);
    HIGH = U32(CoreGen_CLINT_BASE + CoreGen_MTIME_CMP_ADDR_H);
    mtime_cmp_read = (HIGH << 32) | LOW;
    return mtime_cmp_read;
}

void CLINT_mtime_cmp_write(uint64_t mtime_cmp_num)
{
    unsigned int LOW, HIGH;

	mtime_cmp_num += CLINT_mtime_read();

    LOW  = mtime_cmp_num;
    HIGH = mtime_cmp_num >> 32;
    U32(CoreGen_CLINT_BASE + CoreGen_MTIME_CMP_ADDR_L)  = LOW ;
    U32(CoreGen_CLINT_BASE + CoreGen_MTIME_CMP_ADDR_H) = HIGH;
}

//--------------------------------------------------------------------------
// Main�?�'��?�?��?��?�?�??4�?��??

void clint_init(void)
{
    //uint64_t time;
    //CLINT_mtime_write(0);
    CLINT_mtime_cmp_write(MY_TIME);
    clint_csr_cfg();
    CLINT_msip_write(0);

    //while(1) {
    //    asm volatile ("wfi");
    //}
}
//uint32 mepc_back;
void simple_exc_handler(void)
{
    uint32_t cause;
    cause = get_mcause();
    cause = cause & 0x1f;
    if(cause == 7){
        handle_trap_timer();
    }
    else if(cause == 3){
        handle_trap_software();
    }
    else {
        
    }
}

void __attribute__((interrupt)) simple_exc_handler_fast(void)
{
	simple_exc_handler();
}

unsigned int get_mcause() {
  uint32_t result;
  __asm__ volatile("csrr %0, mcause;" : "=r"(result));
  return result;
}



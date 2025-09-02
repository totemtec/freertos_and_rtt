#include "encoding.h"
#include <stdint.h>

#include "my_typedef.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"

#define U32         *(volatile unsigned int *)
#define CoreGen_CLINT_BASE   0x00060000
typedef unsigned long long   uint64_t;
long hw_interrupt_enable(long level);
long hw_interrupt_disable();
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
void CLINT_misp_write(unsigned int misp_num);
void timer_init(void);

void simple_exc_handler();
// handle_trap function
extern void trap_entry(void);
extern void trap_end(void);
unsigned int get_mcause();


#include "FreeRTOS.h"


int tm1;

#define MY_TIME 40000

void test_pass(void);
//--------------------------------------------------------------------------


void set_mmode(void)
{
	asm volatile( "li t0, 0x00001800" );
  	asm volatile( "csrs mstatus, t0" );
}

void handle_trap_timer()
{    
    // unsigned int csr_tmp;
    // //mie.MEIE
    // unsigned int csr_mstaus;
    // unsigned int csr_mip;

    //timer_num1 = timer_num1 + 1;

    //rt_interrupt_enter();
    //rt_tick_increase();
    //rt_interrupt_leave();

   

    // GET_INT_SP();
	//portDISABLE_INTERRUPTS();

    tm1++;

    CLINT_mtime_write(0);
    CLINT_mtime_cmp_write(MY_TIME);
    
    
    if( xTaskIncrementTick() != pdFALSE )
    {
        //ee_printf("InTick fine\n");
        //vTaskSwitchContext();
        portYIELD();
        tm1 += 10000;
    }

    // portENABLE_INTERRUPTS();
    // FREE_INT_SP();

    //set_mmode();
    
    //csr_tmp = read_csr(mie);
    // write_csr(mie,(csr_tmp | 0xFFFF0F0F));//
   
    //csr_mstaus = read_csr(mstatus);
    //csr_mip    = read_csr(mip);
    //ee_printf( "mip: %d\n",csr_mip);
    //ee_printf( "mstaus: %d\n",csr_mstaus);

    return;
}

int g_MIE;
int g_MSTATUS;

void TEST(void)
{
	g_MIE = read_csr(mie);
	g_MSTATUS = read_csr(mstatus);
}

long g_level;
void DIS_I(void)
{
	g_level = hw_interrupt_disable();
}
void EN_I(void)
{
	g_level = 136;

	hw_interrupt_enable(g_level);
}


void handle_trap_software()
{
    unsigned int csr_tmp;
    //csr_tmp = read_csr(mie);
    //write_csr(mie,(csr_tmp | 0xFFFF0FF0));
    CLINT_misp_write(0);

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

/********************************************/
//mtime write and read 
uint64_t CLINT_mtime_read()
{
    uint64_t LOW, HIGH;
    uint64_t mtime_read;
    LOW = U32(CoreGen_CLINT_BASE + 0);
    HIGH = U32(CoreGen_CLINT_BASE + 4); 
    mtime_read = (HIGH << 32) | LOW;
    return mtime_read;
}

void CLINT_mtime_write(uint64_t mtime_num)
{
    unsigned int LOW, HIGH;
    LOW  = mtime_num;
    HIGH = mtime_num >> 32;
    U32(CoreGen_CLINT_BASE + 0) = LOW  ;
    U32(CoreGen_CLINT_BASE + 4) = HIGH ;
}
/**********************************************/
//software write
void CLINT_misp_write(unsigned int misp_num)
{
    U32(CoreGen_CLINT_BASE + 0x10) = misp_num;
}
/*********************************************/
//mtime write and read
uint64_t CLINT_mtime_cmp_read()
{
    uint64_t LOW, HIGH;
    uint64_t mtime_cmp_read;
    LOW = U32(CoreGen_CLINT_BASE + 8);
    HIGH = U32(CoreGen_CLINT_BASE + 12); 
    mtime_cmp_read = (HIGH << 32) | LOW;
    return mtime_cmp_read;
}

void CLINT_mtime_cmp_write(uint64_t mtime_cmp_num)
{
    unsigned int LOW, HIGH;
    LOW  = mtime_cmp_num;
    HIGH = mtime_cmp_num >> 32;
    U32(CoreGen_CLINT_BASE + 8)  = LOW ;
    U32(CoreGen_CLINT_BASE + 12) = HIGH;
}

//--------------------------------------------------------------------------
// Main

void clint_init(void)
{
    //uint64_t time;
    CLINT_mtime_write(0);
    CLINT_mtime_cmp_write(MY_TIME);
    clint_csr_cfg();
    CLINT_misp_write(0);

    //while(1) {
    //    asm volatile ("wfi");
    //}

	//DIS_I();
	//EN_I();
}
//uint32 mepc_back;
//void simple_exc_handler(void) __attribute__((interrupt()));
void simple_exc_handler(void){

    uint32_t cause;
    cause = get_mcause();
    cause = cause & 0x1f;
    if(cause == 7){
        //ee_printf("timer call back\n");
        handle_trap_timer();
        //mepc_back = read_csr(mepc);
    }
    else if(cause == 3){
        ee_printf("software call back\n");
        handle_trap_software();
        //write_csr( mepc, mepc_back );
    }
    else {
        //...
    }
}


unsigned int get_mcause() {
  uint32_t result;
  __asm__ volatile("csrr %0," "mcause" : "=r"(result));
  return result;
}
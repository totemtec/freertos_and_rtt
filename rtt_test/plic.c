#include <stdint.h>
#include "plic.h"

volatile ext_int_func_ptr_t ext_int_func_ptr[PLIC_NUM_INTERRUPTS];

void ext_int_default_interrupt_handler (void) 
{
    // puts("you need overwrite this irq isr\n");
}

void  __attribute__ ((interrupt)) external_handler(void)
{
    uint32_t id = PLIC_get_claim();
    ext_int_func_ptr[id]();
    PLIC_set_completed(id);
}

void set_exit_handler(uint32_t id, ext_int_func_ptr_t handler)
{
    ext_int_func_ptr[id] = handler;
}

void PLIC_init()
{
    /* set all interrupts to default handler */
    for (uint32_t i = 0; i < PLIC_NUM_INTERRUPTS; i++)
    {
        ext_int_func_ptr[i] = ext_int_default_interrupt_handler;
    }
    
    /* set all interrupts priorities to 0 */
    for (uint32_t i = 0; i < PLIC_NUM_INTERRUPTS; i++)
    {
        reg32(PLIC_BASE + PLIC_PRIORITY_BASE + i * 4) = 0;
    }

    /* disable all interrupts */
    for (uint32_t i = 0; i < PLIC_NUM_INTERRUPTS/32; i++)
    {
        reg32(PLIC_BASE + PLIC_ENABLE_BASE + i * 4) = 0;
    }
    
    /* set priority threshold to 0 */
    reg32(PLIC_BASE + PLIC_PRIORITY_THRESHOLD) = 0;
}

void PLIC_set_priority(uint32_t interrupt, uint32_t priority)
{
    reg32(PLIC_BASE + PLIC_PRIORITY_BASE + interrupt * 4) = priority;
}

uint32_t PLIC_get_pending(uint32_t interrupt)
{
    uint32_t offset = interrupt / 32;
    uint32_t mask = interrupt % 32;
    uint32_t pend_reg = reg32(PLIC_BASE + PLIC_PENDING_BASE + offset * 4);
    return (pend_reg >> mask) & 0x1;
}

void PLIC_enable_interrupt(uint32_t interrupt)
{
    uint32_t offset = interrupt / 32;
    uint32_t mask = interrupt % 32;
    reg32(PLIC_BASE + PLIC_ENABLE_BASE + offset * 4) |= (1 << mask);
}

void PLIC_disable_interrupt(uint32_t interrupt)
{
    uint32_t offset = interrupt / 32;
    uint32_t mask = interrupt % 32;
    reg32(PLIC_BASE + PLIC_ENABLE_BASE + offset * 4) &= ~(1 << mask);
}

void PLIC_set_threshold(uint32_t threshold)
{
    reg32(PLIC_BASE + PLIC_PRIORITY_THRESHOLD) = threshold;
}
uint32_t PLIC_get_threshold(void)
{
    return reg32(PLIC_BASE + PLIC_PRIORITY_THRESHOLD);
}

uint32_t PLIC_get_claim(void)
{
    return reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED);
}

void PLIC_set_completed(uint32_t id)
{
    reg32(PLIC_BASE + PLIC_CLAIM_COMPLETED) = id;
}



void PLIC_init_core_csr(void){

    unsigned int csr_tmp;

    //mie.MEIE
    csr_tmp = read_csr(mie);

    //write_csr(mie,0x0);
    write_csr(mie,(csr_tmp | 0xFFFF0800));

    //mstatus.MIE
    csr_tmp = read_csr(mstatus);

    //write_csr(mstatus,0x0);
    write_csr(mstatus,(csr_tmp | 0x8));
}

void PLIC_sourceX_cfg(unsigned int context, unsigned int num, unsigned int priority, unsigned int enable)
{
    //set source X priority
    reg32(PLIC_BASE + 0x0000 + 4*num) = priority;
    //set source X enable
    reg32(PLIC_BASE + 0x2000 + context * 124) = reg32(PLIC_BASE + 0x2000 + context * 124) | (enable<<num);
}

void PASS(void) {
    asm volatile(
        "li  x3  , 1\n"  
        "li  x10 , 0\n"
        "ecall\n"      
    );
}

void FAIL(void) {
    asm volatile(
        "li  x3  , 0\n"  
        "li  x10 , 0\n"
        "ecall\n"      
    );
}
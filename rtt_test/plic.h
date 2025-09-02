#ifndef __PLIC_H
#define __PLIC_H
#include <stdint.h>
#include "encoding.h"
// #include "chip_define.h"

#define reg32 *(volatile unsigned int *)

#define PLIC_NUM_INTERRUPTS     256
#define PLIC_NUM_PRIORITIES     8

#define PLIC_PRIORITY_BASE          0x00000000
#define PLIC_PENDING_BASE           0x00001000
#define PLIC_ENABLE_BASE            0x00002000
#define PLIC_PRIORITY_THRESHOLD     0x00200000
#define PLIC_CLAIM_COMPLETED        0x00200004

#define PLIC_BASE   0x01000000

#define CMT0_BASE                   0x90001000
#define CMT0_MISP_REG               0x10
#define CMT0_MTIME_REG_L            0x0
#define CMT0_MTIME_REG_H            0x4
#define CMT0_MTIME_CMP_REG_L        0x8
#define CMT0_MTIME_CMP_REG_H        0x12
#define CMT0_MTIME_HOLD             0x24

typedef void (*ext_int_func_ptr_t) (void);

void PLIC_init();

void PLIC_set_priority(uint32_t interrupt, uint32_t priority);
uint32_t PLIC_get_threshold(void);
uint32_t PLIC_get_pending(uint32_t interrupt);

void PLIC_enable_interrupt(uint32_t interrupt);

void PLIC_disable_interrupt(uint32_t interrupt);

void PLIC_set_threshold(uint32_t threshold);

uint32_t PLIC_get_claim(void);

void PLIC_set_completed(uint32_t id);

void PLIC_init_core_csr(void);

void PLIC_sourceX_cfg(unsigned int context, unsigned int num, unsigned int priority, unsigned int enable);//设置某个id的优先级和使能的值

void PASS();

void FAIL();

#endif
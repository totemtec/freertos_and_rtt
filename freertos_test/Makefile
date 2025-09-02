RISCV_ARCH := rv32gcv
RISCV_ABI := ilp32d
RISCV_MCMODEL := medlow


TARGET = freertos_test

#CFLAGS += -DSIMULATION
#CFLAGS += -O2
#ASM_SRCS +=
#LDFLAGS +=
#INCLUDES += -I.

# INCLUDES := 

INCLUDES += -Itask -I. -IFreeRTOS -IFreeRTOS/include -ICore -IFreeRTOS/portable/GCC/RISC-V -IFreeRTOS/portable/GCC/RISC-V/chip_specific_extensions/RV32I_PFIC_no_extensions/ -Iinc

#C_SRCS := \
#	main.c \
#	clint.c \

# ASM_SRCS := $(wildcard myrtt/libcpu/risc-v/coregen_port/*.S)
# C_SRCS := $(wildcard *.c) $(wildcard task/*.c) $(wildcard myrtt/*.c) $(wildcard myrtt/src/*.c) $(wildcard myrtt/libcpu/risc-v/common/*.c)

ASM_SRCS := $(wildcard FreeRTOS/portable/GCC/RISC-V/*.S)   $(wildcard task/task_rvv/*.S)
C_SRCS := $(wildcard *.c) $(wildcard FreeRTOS/*.c) $(wildcard FreeRTOS/portable/GCC/RISC-V/*.c) $(wildcard FreeRTOS/portable/MemMang/*.c) $(wildcard Core/*.c) $(wildcard task/*.c) $(wildcard task/task_rvv/*.c)

#	/myrtt/libcpu/risc-v/common/*.c \
#	/myrtt/src/*.c \
#	/myrtt/*.c \


COMMON_DIR = ../common
COMMON_LINK_DIR = ../common
TAR_NAME = freertos_test
include ../common.mk


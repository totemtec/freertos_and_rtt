
# use rvv task
RISCV_ARCH := rv32gcv
RISCV_ABI := ilp32d

# use gcc-interrupt
# RISCV_ARCH := rv32imaf
# RISCV_ABI := ilp32f

RISCV_MCMODEL := medlow


TARGET = rtt_test

#CFLAGS += -DSIMULATION
#CFLAGS += -O2
#ASM_SRCS +=
#LDFLAGS +=
#INCLUDES += -I.

# INCLUDES := 

INCLUDES += -Itask -I. -Imyrtt/include -Imyrtt -Imyrtt/libcpu/risc-v/common -Iinc -Itask1/motor/FOC -Iinc -Itask1/motor

#C_SRCS := \
#	main.c \
#	clint.c \

ASM_SRCS := $(wildcard myrtt/libcpu/risc-v/coregen_port/*.S) $(wildcard Atom.S) $(wildcard task_rvv/vec-vadd.S) $(wildcard task_rvv/vec-vmul.S)
# $(wildcard task_rvv/vec-vadd.S) $(wildcard task_rvv/vec-vmul.S)

C_SRCS := $(wildcard *.c) $(wildcard task/*.c) $(wildcard myrtt/*.c) $(wildcard myrtt/src/*.c) $(wildcard myrtt/libcpu/risc-v/common/*.c) $(wildcard task_rvv/*.c)
# $(wildcard task_rvv/*.c)

# C_SRCS += $(wildcard task1/motor/*.c) $(wildcard task1/motor/FOC/*.c)

#	/myrtt/libcpu/risc-v/common/*.c \
#	/myrtt/src/*.c \
#	/myrtt/*.c \


COMMON_DIR = ../common
COMMON_LINK_DIR = common
TAR_NAME = rtt_test
include ../common.mk


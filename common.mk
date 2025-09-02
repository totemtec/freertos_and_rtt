

# use vector test
RISCV := /nfs/home/daiqisen/01_work/riscv-tools/bin/riscv32-unknown-elf-

# use gcc-interrupt
# RISCV := /nfs/app/riscv_32f/bin/riscv32-unknown-elf-

# RISCV := riscv32-unknown-elf-

RISCV_GCC     := $(RISCV)gcc
RISCV_AS      := $(RISCV)as
RISCV_GXX     := $(RISCV)g++
RISCV_OBJDUMP := $(RISCV)objdump
RISCV_GDB     := $(RISCV)gdb
RISCV_AR      := $(RISCV)ar
RISCV_OBJCOPY := $(RISCV)objcopy
RISCV_READELF := $(RISCV)readelf

.PHONY: all
all: $(TARGET) link_myfile

# ASM_SRCS += $(COMMON_DIR)/start.S
ASM_SRCS += $(COMMON_LINK_DIR)/start.S

C_SRCS += $(COMMON_DIR)/../lib/utils.c
C_SRCS += $(COMMON_DIR)/../lib/uart.c
C_SRCS += $(COMMON_DIR)/../lib/htif.c

LINKER_SCRIPT := $(COMMON_LINK_DIR)/link.ld

INCLUDES += -I../include -I../lib/ -I../common/

LDFLAGS += -T $(LINKER_SCRIPT) -nostartfiles -Wl,--gc-sections -Wl,--check-sections

ASM_OBJS := $(ASM_SRCS:.S=.o)
C_OBJS := $(C_SRCS:.c=.o)

LINK_OBJS += $(ASM_OBJS) $(C_OBJS)
LINK_DEPS += $(LINKER_SCRIPT)

CLEAN_OBJS += $(TARGET) $(LINK_OBJS) $(TARGET).dump $(TARGET).verilog

CFLAGS += -march=$(RISCV_ARCH) -Wl,-Map=$(TARGET).map
CFLAGS += -mabi=$(RISCV_ABI)
CFLAGS += -mcmodel=$(RISCV_MCMODEL) -ffunction-sections -fdata-sections -fno-builtin-printf -fno-builtin-malloc

$(TARGET): $(LINK_OBJS) $(LINK_DEPS) Makefile
	$(RISCV_GCC) $(CFLAGS) $(INCLUDES) $(LINK_OBJS) -o $@ $(LDFLAGS)
	$(RISCV_OBJCOPY) -O verilog $@ $@.verilog
	$(RISCV_OBJDUMP) --disassemble-all $@ > $@.dump


$(ASM_OBJS): %.o: %.S
	$(RISCV_GCC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(C_OBJS): %.o: %.c
	$(RISCV_GCC) $(CFLAGS) $(INCLUDES) -c -o $@ $< -lm -lgcc

UART_TEST=../../sim_room/test_case/$(TAR_NAME)
SOME_DIR=../../../smoke_test/test_case/core_memory

LINK_NAME:=$(TAR_NAME)

link_myfile:
	@if [ ! -d $(UART_TEST) ]; then \
		mkdir $(UART_TEST); \
	fi
	rm -rf $(UART_TEST)/* $(SOME_DIR)/$(LINK_NAME) $(SOME_DIR)/$(LINK_NAME).dump $(SOME_DIR)/$(LINK_NAME).verilog
	link $(LINK_NAME) $(UART_TEST)/$(LINK_NAME)
	link $(LINK_NAME).dump $(UART_TEST)/$(LINK_NAME).dump
	link $(LINK_NAME).verilog $(UART_TEST)/$(LINK_NAME).verilog
	cp -r $(LINK_NAME) $(SOME_DIR)
	cp -r $(LINK_NAME).dump $(SOME_DIR)
	cp -r $(LINK_NAME).verilog $(SOME_DIR)

.PHONY: clean
clean:
	rm -f $(CLEAN_OBJS)




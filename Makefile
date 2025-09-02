##**************************************************##
##													##
##					FIND_FFILE						##
##													##
##**************************************************##
SV_DIR := ../../../sv_rtl
JTAG_dir := ./jtag
#@pz: 2023.8.1 22:23 change to dv_utils;add 13 for "dv_fcov_macros.svh"
find_sv:
	@rm -rf filelist.f | touch filelist.f
	@echo +incdir+${SV_DIR}/core_pkg/defines >> filelist.f
	@echo +incdir+./tb >> filelist.f
	@echo +incdir+${SV_DIR}/system/defines >> filelist.f
	@echo +incdir+../../../DV/uvm/lowrisc_ip/dv/sv/dv_utils >> filelist.f
	# @echo +incdir+${SV_DIR}/vendor/pulp_platform_common_cells/include/common_cells >> filelist.f
	@echo -CFLAGS -I../../${JTAG_dir}/jtagdpi >> filelist.f
	@find ${JTAG_dir}/tcp -name "*.c" >> filelist.f
	@find ${JTAG_dir}/jtagdpi -name "*.sv" >> filelist.f
	@find ${JTAG_dir}/jtagdpi -name "*.c" >> filelist.f
	@find ./bosc-dpi -name "dpi.c" >> filelist.f
	@find ${SV_DIR} -name "*.vh" >> filelist.f
	@echo "./tb/simple_soc_params.vh" >> filelist.f
	@find ${SV_DIR} -name "*.svh" >> filelist.f
	@find ${SV_DIR} -name "*.sv" -o -name "*.v" >> filelist.f
	@echo "./tb/tty.sv" >> filelist.f
	@echo "./tb/tb_simple_system.sv" >> filelist.f
	@echo "filelist.f is update"

##**************************************************##
##													##
##					COMPILE  						##
##													##
##**************************************************##
BUILD_DIR := ./build
VCS_DEBUG := -fsdb -debug_access

compile:find_sv
	@if [ ! -d $(BUILD_DIR) ]; then \
		mkdir $(BUILD_DIR); \
	fi
	vcs -full64 -assert svaext -sverilog +v2k -timescale=1ns/1ns \
	+vcs+loopreport+10000 \
	-CFLAGS -I../../jtag/tcp \
	$(VCS_DEBUG) \
	+lint=TFIPC-L \
	-daidir=$(BUILD_DIR)/simv.daidir -Mdir=$(BUILD_DIR)/csrc \
	-l $(BUILD_DIR)/compile.log -f filelist.f

##**************************************************##
##													##
##					SIGNAL_TEST						##
##													##
##**************************************************##


MY_TESTNAME = core_test
SMOKE_DIR ?= ../../smoke_test/test_case/core_memory


TEST_DIR := ./test_case/$(MY_TESTNAME)
TEST_CASE := $(MY_TESTNAME)
INIT_RESULT := $(BUILD_DIR)/result/init_test
VERDI_FILE := $(BUILD_DIR)/result/init_test/$(TEST_CASE)
#@pz: 2023.8.1 22:23 change the trace_file;+TRACE_FILE=$(BUILD_DIR)/init_test/$(TEST_CASE).trace
vcs: compile
	@if [ ! -d $(INIT_RESULT) ]; then \
		mkdir -p $(INIT_RESULT); \
	fi
	 ./simv \
	 simv +ntb_random_seed_automatic \
	  +SRAM_FILE=$(TEST_DIR)/$(TEST_CASE).verilog \
	  +FSDB_FILE=$(INIT_RESULT)/$(TEST_CASE).fsdb \
	  +TRACE_FILE=$(INIT_RESULT)/$(TEST_CASE).trace \
	  -l $(BUILD_DIR)/$(TEST_CASE).log

verdi:
	 verdi  -sverilog -nologo \
	  -ssf $(VERDI_FILE).fsdb -f filelist.f

##**************************************************##
##													##
##					DEBUG_TEST						##
##													##
##**************************************************##
build:
	make -C ../generate_case/$(MY_TESTNAME) clean all

myc:clean build
myr:clean build vcs

FILE_NAME ?= $(MY_TESTNAME)
MYTEST_DIR ?= ../generate_case/$(MY_TESTNAME)

mycopy:
	cp -rf $(MYTEST_DIR)/$(FILE_NAME) $(SMOKE_DIR)
	cp -rf $(MYTEST_DIR)/$(FILE_NAME).dump $(SMOKE_DIR)
	cp -rf $(MYTEST_DIR)/$(FILE_NAME).verilog $(SMOKE_DIR)

##**************************************************##
##													##
##					DEBUG_TEST						##
##													##
##**************************************************##
clean:
	@-rm -rf simv* ucli.key $(BUILD_DIR) ./*.log ./novas.* ./verdiLog csrc
	@-rm -rf $(RV32I_RESULT) $(RV32M_RESULT) $(RV32C_RESULT) $(RV32F_RESULT) $(RV32D_RESULT) $(RV32Z_RESULT)
.PHONY: compile vcs verdi smoke clean




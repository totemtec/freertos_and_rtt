# Build

Ubuntu24
xpack-riscv-none-elf-gcc-14.2.0-3

```sh
cd freertos_test
make clean
make
```

# Run on Windows

### Copy binary to Windows

smoke_test/test_case/core_memory/freertos_test

copy to 

D:\BoscV1301a\toolpaths\riscv-none-elf-gcc\bin

### Start OpenOCD

```sh

cd D:\Projects\sdk-im110gw-v2-fullmask_boscv\tools_make\openocd

.\openocd.exe -f ./im110_gw-builtin.cfg -c "reset halt" -c "reg pc 0x40000000"



Open On-Chip Debugger 0.12.0+dev-gf13576f96-dirty (2025-07-29-11:43)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : auto-selecting first available session transport "jtag". To override use 'transport select <transport>'.
Newtap done.
Info : flash clk = 6
Start to scan chain.
Info : clock speed 500 kHz
Info : JTAG tap: riscv.cpu tap/device found: 0x249511c5 (mfg: 0x0e2 (Super PC Memory), part: 0x4951, ver: 0x2)
Info : auth path: D:/Projects/sdk-im110gw-v2-fullmask_boscv/tools_make/openocd/tcl/target/im110_auth.txt
Info : datacount=2 progbufsize=8
Info : Vector support with vlenb=4
Info : Examined RISC-V core; found 1 harts
Info :  hart 0: XLEN=32, misa=0x40301124
Info : [riscv.cpu] Examination succeed
Info : [riscv.cpu] starting gdb server on 3333
Info : Listening on port 3333 for gdb connections
Ready for Remote Connections.
Info : JTAG tap: riscv.cpu tap/device found: 0x249511c5 (mfg: 0x0e2 (Super PC Memory), part: 0x4951, ver: 0x2)
pc (/32): 0x40000000
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections

```

### Start GDB

```sh

cd D:\BoscV1301a\toolpaths\riscv-none-elf-gcc\bin

.\riscv-none-elf-gdb.exe .\freertos_test

(gdb) target remote localhost:3333

```

### Error

```sh
(gdb) target remote localhost:3333
Remote debugging using localhost:3333
bfd requires flen 8, but target has flen 4
```

##### 解决方法

- Make前需要将 abi 参数修改为 RISCV_ABI := ilp32 代替 原来的 ilp32d，相关修改可以查看 git 日志

也尝试过设置gdb参数，好像没啥用
```sh
(gdb) set arch riscv:rv32
```
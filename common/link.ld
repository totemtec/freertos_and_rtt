
OUTPUT_ARCH(riscv)

MEMORY
{
    /* 128 kB should be enough for anybody... */
    /* @pz this addr is the ahbl_memory 0x2000_0000 */
    /* if you want to choose the itcm is 0x0000_0000 */
    /* itcm(RWX)   : ORIGIN = 0x20000000, LENGTH = 0x20000 /* 128 kB */
    /* dtcm(RWX)   : ORIGIN = 0x20020000, LENGTH = 0x18000 /* 96 kB */
    /* stack(RWX)  : ORIGIN = 0x20038000, LENGTH = 0x8000  /* 32 kB */

	
	/* spike need this config! if 0x00 will error: Memory address 0x5e68 is invalid */
	/* itcm        : ORIGIN = 0x00100000, LENGTH = 0x20000 /* 128 kB */

	/* vcs need this config! */
	/* if ahbl, the base addr is 0x20000000 !!! core is 0x00000000 */
	itcm        : ORIGIN = 0x00000000, LENGTH = 0x20000 /* 128 kB */

	dtcm        : ORIGIN = 0x00020000, LENGTH = 0x18000 /* 96 kB */
	stack       : ORIGIN = 0x00038000, LENGTH = 0x8000  /* 32 kB */
}

/* Stack information variables */
_stack_len     = LENGTH(stack);
_stack_start   = ORIGIN(stack) + LENGTH(stack);
_stack_end     = ORIGIN(stack) + LENGTH(stack);

ENTRY(__start)

SECTIONS
{
    .text : {
        *(.text.start)
        *(.text*)
        . = ALIGN(4);
    }  > itcm

	.uartif :
    {
        PROVIDE( __uartif_base = . );
        *(.uartif)
    } > dtcm

    .rodata : {
        . = ALIGN(4);
        /* Small RO data before large RO data */
        *(.srodata)
        *(.srodata.*)
        *(.rodata);
        *(.rodata.*)
    } > itcm

	.htif :
    {
        PROVIDE( __htif_base = . );
        *(.htif)
    } > dtcm
    . = ALIGN(0x1000);

    .data : {
        . = ALIGN(4);
        //__global_pointer$ = . + 0x800;
        /* Small data before large data */
        *(.sdata)
        *(.sdata.*)
        *(.data);
        *(.data.*)
    } > dtcm

    .myinterrupt :
    {
        . = ALIGN(16);  /* note: freeRTOS need aligned 16, see portmacro.h 88 line, rtt is 4 */
        _myinterrupt_start = .;
        . = . + 2048;
        _myinterrupt_end = .;
		__freertos_irq_stack_top = .;  /* note: freeRTOS need aligned 16, see portmacro.h 88 line */
    } > dtcm

    .bss :
    {
        . = ALIGN(4);
        _bss_start = .;
        /* Small BSS before large BSS */
        *(.sbss)
        *(.sbss.*)
        *(.bss)
        *(.bss.*)
        *(COMMON)
        _bss_end = .;
    } > dtcm

    /* ensure there is enough room for stack */
    .stack (NOLOAD): {
        . = ALIGN(4);
        stack = . ;
        _stack = . ;
    } > stack

    /* End of uninitalized data segement */
    _end = .;
}

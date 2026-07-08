ENTRY(boot2)

MEMORY
{
    boot2   (rx)    : ORIGIN = 0x10000000, LENGTH = 256
    flash   (rx)    : ORIGIN = 0x10000100, LENGTH = 2040k
    sram    (rwx)   : ORIGIN = 0x20000000, LENGTH = 256k
    stack_x (rwx)   : ORIGIN = 0x20040000, LENGTH = 4k
    stack_y (rwx)   : ORIGIN = 0x20041000, LENGTH = 4k
}

SECTIONS
{   
    .boot2 :
    {
        KEEP(*(.boot2))
        . = ORIGIN(boot2) + 252;
        LONG(0x00000000)
    } > boot2

    .vector : ALIGN(256)
    {
        _vector_st = .;
        KEEP(*(.vector))
        . = ALIGN(4);
        _vector_end = .;
    } > sram AT > flash
    _vector_src = LOADADDR(.vector);

    .text : ALIGN(4)
    {
        *(.text*)
    } > flash

    .rodata : ALIGN(4)
    {
        *(.rodata*)
    } > flash

    .data : ALIGN(4)
    {
        _data_st = .;
        *(.data*)
        . = ALIGN(4);
        _data_end = .;
    } > sram AT > flash
    _data_src = LOADADDR(.data);

    .bss (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        _bss_st = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > sram

    .heap (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4);
        _heap_st = .;
        . = . + 8k;
        _heap_end = .;
    } > sram

    .stack0 (NOLOAD) :
    {
        _stack0_bot = .;
        . = . + LENGTH(stack_x);
        _stack0_top = .;
    } > stack_x

    .stack1 (NOLOAD) :
    {
        _stack1_bot = .;
        . = . + LENGTH(stack_y);
        _stack1_top = .;
    } > stack_y

}


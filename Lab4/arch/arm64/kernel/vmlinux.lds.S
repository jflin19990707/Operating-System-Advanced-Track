SECTIONS
{
    text_begin = .;
    .text.boot : { *(.text.boot) }
    .text : { *(.text) }
    text_end = .;
    . = ALIGN(0x1000);
    rodata_begin = .;
    .rodata : { *(.rodata) }
    rodata_end = .;
    . = ALIGN(0x1000);
    data_begin = .;
    .data : { *(.data) }
    data_end = .;
    . = ALIGN(0x1000);
    __bss_start = .;
    .bss : { *(.bss*) }
    __bss_end = .;
    . = ALIGN(0x1000);
    pg_dir = .;
    . = 0x68000;
    stack_top = .;
    . = 0x80000;
    kernel_end = .;
    __bss_size = __bss_end - __bss_start;
}


ENTRY(_start)

SECTIONS {
    . = 1M;
    _loader_start = .;

    .headers ALIGN(4K) : ALIGN(16) { KEEP(*(.headers)) }
    .text ALIGN(4K) : { *(.text .text.*) }
    .rodata ALIGN(4K) : { *(.rodata .rodata.*) }
    .bss ALIGN(4K) : {
        *(.bss .bss.*)
    }

    _loader_end = .;
}

.code64

.extern halt

.global _main64
.section .text
.type _main64, @function

_main64:
    // load null (0) into some cpu data registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    xor rbp, rbp

    // execute our c++ code
    call kernel_main
    call halt

.code32 // we are using x86 at the time of boot

.section .headers, "a"
.align 8

// this header is searched for by the bootloader in the first 32 kib (32768 bytes) of the kernel
// this allows a bootloader to detect if we are bootable or not
.multiboot_header:
    .long 0xE85250D6 // magic number, used to see if we are a multiboot2 compatible kernel
    .long 0 // architecture, 0 is low endian and 4 is big endian
    .long .multiboot_header_end - .multiboot_header // header length
    .long -(0xE85250D6 + (.multiboot_header_end - .multiboot_header)) // checksum

    /* end tag */
    .short 0
    .short 0
    .long 8

.multiboot_header_end:

// this is our entry point specified in our kernel.lds
// the bootloader will jump to here when the kernel has been loaded
.section .text
.type _start, @function

.global _start
.global halt

// these are our subroutines declared in other files
.extern _main64
.extern check_cpuid
.extern check_long_mode
.extern setup_paging
.extern enable_paging

_start:
    // here, we will setup our stack by moving the esp register to the top of the stack
    lea esp, [.stack_top]

    // check if we can enable long mode
    call check_cpuid
    call check_long_mode

    // enable x64 cpu paging
    call setup_paging
    call enable_paging

    // after we enable paging, we can now load the global descriptor table
    lgdt [.pointer]
    // load our code segment into the code sector
.att_syntax prefix
    ljmp $0x8, $_main64
.intel_syntax noprefix

    // once the system has nothing else to do, halt
    call halt

// set the size of the _start symbol to the current location '.' minus its the start of it.
.size _start, . - _start

halt:
1:  hlt
    jmp 1b

.section .bss
.align 4096 // aligning to a number means the eventual memory address will be divisible by it

// this is our stack
// we have to define the size of the stack as multiboot does not do it for us
// esp is the stack register
// we create a symbol at the bottom and allocate 16384 bytes to it, then create a symbol at the top
// note: the stack grows downwards on the x86 architecture
// note: the stack is 16 byte aligned as stated by the system v ABI standard
stack_bottom:
    .skip 16384 // this is the 16 kilobytes allocated to our stack
.stack_top:
    .size stack_bottom, . - stack_bottom

.section .rodata
.align 8
// this is our 64 bit global descriptor table
gdt64:
    .quad 0 // 0 entry
.code_segment:
    .quad (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53) | 0xF00000000FFFF
    // here we enabled the executable flag, set the descriptor flag to data segment and enabled the present flag, then
    // we enabled the 64 bit flag

    .size gdt64, . - gdt64
// a pointer to our GDT
.pointer:
    .short .pointer - gdt64
    .long gdt64
    .size .pointer, . - .pointer

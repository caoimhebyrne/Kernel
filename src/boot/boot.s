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
.global _start
.type _start, @function

_start:
    // here, we will setup our stack by moving the esp register to the top of the stack
    lea esp, [.stack_top]

    // check if we can enable long mode
    call check_cpuid
    call check_long_mode

    // we are now in 64 bit mode, enter the kernel
    call kernel_main

    // once the system has nothing else to do, halt
    call halt

// set the size of the _start symbol to the current location '.' minus its the start of it.
.size _start, . - _start

// to check for cpuid, we attempt to flip the id bit of the flags register
// if we are able to flip it, our cpu supports cpuid
check_cpuid:
    pushfd             // push flags register onto the stack
    pop eax            // pop off the stack into the eax register
    mov ecx, eax       // copy of the flags to the ecx register
    xor eax, 1 << 21   // attempt to flip the id bit on the eax register
    push eax           // copy it back onto the stack
    popfd              // pop eax back onto the stack
    pushfd             // put it into the flags register
    pop eax            // copy the flags back into the eax register
    push ecx           // put the value at ecx on the stack
    popfd              // put the value of ecx into the flags register
    cmp eax, ecx       // compare eax and ecx (the original & flipped flags)
    je no_cpuid        // if they match, the cpu did not allow us to flip the bit, cpuid is not available
    ret

.size check_cpuid, . - check_cpuid

// to move in to long mode, we first need to check if the cpu supports it
// if the cpu doesn't support it, we are going to display an error
check_long_mode:
    // check if cpuid supports extended processor info
    mov eax, 0x80000000 // move this into the eax register
    cpuid               // eax is passed to this, if this cpu
                        // supports extended processor info, a number greater
                        // than the one in eax (0x80000000) will be written to it
    cmp eax, 0x80000001 // if eax < 0x80000001
    jb no_long_mode     // eax > 0x80000001

    // check if cpu supports long mode
    mov eax, 0x80000001 // move this into the eax register
    cpuid               // will store a value into the edx register
    test edx, 1 << 29   // test if ln bit is set
    jz no_long_mode     // long mode is not available
    ret

.size check_long_mode, . - check_long_mode

no_cpuid:
    // 0x0f is our color code (white) and Fe is N in hex
    mov dword ptr [0xb8000], 0x0F6e
    mov dword ptr [0xb8002], 0x0F6f
    mov dword ptr [0xb8004], 0x0F63
    mov dword ptr [0xb8006], 0x0F70
    mov dword ptr [0xb8008], 0x0F75
    call halt

.size no_cpuid, . - no_cpuid

no_long_mode:
    // 0x0f is our color code (white) and 6c is L in hex
    mov dword ptr [0xb8000], 0x0F6c
    mov dword ptr [0xb8002], 0x0F6f
    mov dword ptr [0xb8004], 0x0F6e
    mov dword ptr [0xb8006], 0x0F67
    mov dword ptr [0xb8008], 0x0F6d
    call halt

.size no_long_mode, . - no_long_mode

halt:
1:  hlt
    jmp 1b
.size halt, . - halt

// this is our stack
// we have to define the size of the stack as multiboot does not do it for us
// esp is the stack register
// we create a symbol at the bottom and allocate 16384 bytes to it, then create a symbol at the top
// note: the stack grows downwards on the x86 architecture
// note: the stack is 16 byte aligned as stated by the system v ABI standard
.section .bss
.align 16
stack_bottom:
.skip 16384 // this is the 16 kilobytes allocated to our stack
.stack_top:
.size stack_bottom, . - stack_bottom
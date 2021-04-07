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
.extern _main64

.type _start, @function

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

setup_paging:
    lea eax, [page_table_l3]    // take the address of the level 3 page table and move it into the eax register
                              // the first 12 bits of every entry is always going to be 0 (log2 4096 = 12)
    or eax, 0b11              // enable the present and writable flags
    mov [page_table_l4], eax  // take the address in eax (which has some added flags) and put it as the first entry in our l4 table

    // the code is the same as before, just different tables
    lea eax, [page_table_l2]
    or eax, 0b11
    mov [page_table_l3], eax

    // allocate 1gb in the l2 page table to identity map
    mov ecx, 0  // this is our counter
.loop:
    // map a 2mb page
    mov eax, 0x200000                    // store the value of 2MiB in the eax register
    mul ecx                              // get the correct address
    or eax, 0b10000011                   // enable the "huge page" flag
    mov [page_table_l2 + ecx * 8], eax   // put this entry in the l2 table, with the offset of the index of the entry

    inc ecx                              // increment this counter
    cmp ecx, 512                         // check if the whole table has been mapped
    jne .loop                            // if not, continue looping
    ret

enable_paging:
    // pass the page table location to the cpu
    lea eax, [page_table_l4]  // pass the page table location to eax
    mov cr3, eax            // copy this value into the cr3 register

    // enable physical address extension
    mov eax, cr4            // copy the cr4 register to eax
    or eax, 1 << 5          // enable the 5th bit which is the PAE flag
    mov cr4, eax            // enable the PAE flag in the cr4 register

    // enable long mode
    mov ecx, 0xC0000080     // put the magic value in to the ecx register
    rdmsr                   // read model specific register, this will load the value of the efer register into eax
    or eax, 1 << 8          // enable long mode flag
    wrmsr                   // write eax back into the efer register

    // enable paging
    mov eax, cr0            // copy the cr0 register to eax
    or eax, 1 << 31         // enable the 'paging bit'
    mov cr0, eax            // write eax to cr0
    ret

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


.section .bss
// aligning to a number means the eventual memory address will be divisible by it
.align 4096
// these are our page tables
// the page table defines the mappings from a virtual address to a physical address
// every time there's a memory access, the cpu checks the tables and finds out where the real destination should be
page_table_l4:
    .skip 4096
page_table_l3:
    .skip 4096
page_table_l2:
    .skip 4096
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

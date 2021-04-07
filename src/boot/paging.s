.code32
// these are our page tables
// the page table defines the mappings from a virtual address to a physical address
// every time there's a memory access, the cpu checks the tables and finds out where the real destination should be
.section .bss
.align 4096
page_table_l4:
    .skip 4096
page_table_l3:
    .skip 4096
page_table_l2:
    .skip 4096

.section .text

.global setup_paging
.global enable_paging

.extern page_table_l4
.extern page_table_l3
.extern page_table_l2

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

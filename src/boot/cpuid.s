.code32
.section .text

.extern halt

.global check_cpuid
.global check_long_mode

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

.size _check_cpuid, . - _check_cpuid

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

.size _check_long_mode, . - _check_long_mode

// error functions


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

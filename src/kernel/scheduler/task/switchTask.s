.global switchTask

.att_syntax prefix
switchTask:
    // Save current registers
    mov %rax, (%rsi)
    mov %rbx, 8(%rsi)
    mov %rcx, 16(%rsi)
    mov %rdx, 24(%rsi)
    mov %rsi, 32(%rsi)
    mov %rdi, 40(%rsi)
    mov %rsp, 48(%rsi)
    mov %rbp, 56(%rsi)
    mov %r8, 64(%rsi)
    mov %r9, 72(%rsi)
    mov %r10, 80(%rsi)
    mov %r11, 88(%rsi)
    mov %r12, 96(%rsi)
    mov %r13, 104(%rsi)
    mov %r14, 112(%rsi)
    mov %r15, 120(%rsi)
    // rflags
    pushf
    pop %rax
    mov %rax, 128(%rsi)

    mov %cr0, %rax
    mov %rax, 136(%rsi)

    mov %cr2, %rax
    mov %rax, 144(%rsi)

    mov %cr3, %rax
    mov %rax, 152(%rsi)

    mov %cr4, %rax
    mov %rax, 160(%rsi)

    mov %cr8, %rax
    mov %rax, 168(%rsi)

    mov %ds, 176(%rsi)
    mov %ss, 178(%rsi)
    mov %fs, 180(%rsi)
    mov %gs, 182(%rsi)

    // restore registers
    mov 182(%rdi), %gs
    mov 180(%rdi), %fs
    mov 178(%rdi), %ss
    mov 176(%rdi), %ds

    mov 168(%rdi), %rax
    mov %rax, %cr8

    mov 160(%rdi), %rax
    mov %rax, %cr4

    mov 152(%rdi), %rax
    mov %rax, %cr3

    mov 144(%rdi), %rax
    mov %rax, %cr2

    mov 136(%rdi), %rax
    mov %rax, %cr0

    mov 128(%rdi), %rax
    push %rax
    popf

    mov 120(%rdi), %r15
    mov 112(%rdi), %r14
    mov 104(%rdi), %r13
    mov 96(%rdi), %r12
    mov 88(%rdi), %r11
    mov 80(%rdi), %r10
    mov 72(%rdi), %r9
    mov 64(%rdi), %r8
    mov 56(%rdi), %rbp
    mov 48(%rdi), %rsp
    mov 40(%rdi), %rdi
    mov 32(%rdi), %rsi
    mov 24(%rdi), %rdx
    mov 16(%rdi), %rcx
    mov 8(%rdi), %rbx
    mov (%rdi), %rax

    ret

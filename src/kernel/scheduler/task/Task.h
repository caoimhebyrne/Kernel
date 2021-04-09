#ifndef KERNEL_TASK_H
#define KERNEL_TASK_H

#include <stddef.h> // NOLINT(modernize-deprecated-headers)

struct Task {
    size_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, r8, r9, r10, r11, r12, r13, r14, r15, rflags, cr0, cr2, cr3, cr4, cr8;
    uint16_t ds, ss, fs, gs;
} __attribute__((packed));

extern "C" void switchTask(Task *task, Task *currentTask);

#endif //KERNEL_TASK_H

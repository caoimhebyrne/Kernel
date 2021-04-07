#ifndef KERNEL_INTERRUPT_FRAME_H
#define KERNEL_INTERRUPT_FRAME_H

#include <stddef.h>

struct interrupt_frame {
    size_t instruction_pointer;
    size_t code_segment;
    size_t flags;
    size_t stack_pointer;
    size_t stack_segment;
};

#endif //KERNEL_INTERRUPT_FRAME_H
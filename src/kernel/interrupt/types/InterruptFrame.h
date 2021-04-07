#ifndef KERNEL_INTERRUPTFRAME_H
#define KERNEL_INTERRUPTFRAME_H

#include <stddef.h> // NOLINT(modernize-deprecated-headers)

struct InterruptFrame {
    size_t instruction_pointer;
    size_t code_segment;
    size_t flags;
    size_t stack_pointer;
    size_t stack_segment;
};

#endif //KERNEL_INTERRUPTFRAME_H
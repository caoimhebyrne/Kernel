#ifndef KERNEL_INTERRUPTFRAME_H
#define KERNEL_INTERRUPTFRAME_H

#include <stddef.h> // NOLINT(modernize-deprecated-headers)

struct InterruptFrame {
    size_t instructionPointer;
    size_t codeSegment;
    size_t flags;
    size_t stackPointer;
    size_t stackSegment;
};

#endif //KERNEL_INTERRUPTFRAME_H
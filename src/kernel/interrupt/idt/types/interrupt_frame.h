#ifndef KERNEL_INTERRUPT_FRAME_H
#define KERNEL_INTERRUPT_FRAME_H

struct interrupt_frame {
    uint16_t instruction_pointer;
    uint16_t code_segment;
    uint16_t flags;
    uint16_t stack_pointer;
    uint16_t stack_segment;
};

#endif //KERNEL_INTERRUPT_FRAME_H
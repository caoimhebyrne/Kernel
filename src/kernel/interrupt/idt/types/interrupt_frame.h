struct interrupt_frame {
    uint16_t instruction_pointer;
    uint16_t code_segment;
    uint16_t flags;
    uint16_t stack_pointer;
    uint16_t stack_segment;
};

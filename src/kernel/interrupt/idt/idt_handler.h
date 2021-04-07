#include "types/idt_descriptor.h"
#include "types/interrupt_frame.h"
#include "../../display/display.h"
#include "../../asm/asm.h"

#ifndef KERNEL_IDT_HANDLER_H
#define KERNEL_IDT_HANDLER_H

class IDTHandler {
public:
    IDTDescr *initialize();

    void initialize_descriptor(uint16_t selector, uint64_t offset, uint16_t type_attr, struct IDTDescr *desc);

private:
    static IDTDescr idt[256];
};

#endif //KERNEL_IDT_HANDLER_H
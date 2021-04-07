#include "types/idt_descriptor.h"
#include "types/interrupt_frame.h"
#include "../../display/display.h"
#include "../../asm/asm.h"

#ifndef KERNEL_IDT_HANDLER_H
#define KERNEL_IDT_HANDLER_H

class IDTHandler {
public:
    /**
     * allows you to register a method that the cpu will call when your specified exception is thrown
     * this function expects that the exception that you are catching does not provide an error code
     *
     * @param selector the selector of the exception
     * @param func a pointer to your function which will be invoked by the cpu
     */
    void register_exception_handler(uint16_t selector, void (*func)(interrupt_frame *)) {
        register_exception_handler(selector, (void (*)(interrupt_frame *, size_t)) func);
    }

    void register_exception_handler(uint16_t selector, void (*func)(interrupt_frame *, size_t));

    void initialize_descriptor(uint64_t offset, uint16_t type_attr, struct IDTDescr *desc);

    IDTDescr *get_idt();

private:
    static IDTDescr idt[256];
};

#endif //KERNEL_IDT_HANDLER_H

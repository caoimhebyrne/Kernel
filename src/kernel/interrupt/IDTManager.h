#include "types/IDTDescriptor.h"
#include "types/InterruptFrame.h"
#include "../display/Display.h"
#include "../asm/asm.h"

#ifndef KERNEL_IDT_MANAGER_H
#define KERNEL_IDT_MANAGER_H

class IDTManager {
public:
    /**
     * allows you to register a method that the cpu will call when your specified exception is thrown
     * this function expects that the exception that you are catching does not provide an error code
     *
     * @param selector the selector of the exception
     * @param func a pointer to your function which will be invoked by the cpu
     */
    void registerExceptionHandler(uint16_t selector, void (*func)(InterruptFrame *)) {
        registerExceptionHandler(selector, (void (*)(InterruptFrame *, size_t)) func);
    }

    void registerExceptionHandler(uint16_t selector, void (*func)(InterruptFrame *, size_t));

    void initializeDescriptor(uint64_t offset, uint16_t type_attr, struct IDTDescriptor *desc);

    IDTDescriptor *getTable();

private:
    static IDTDescriptor table[256];
};

#endif //KERNEL_IDT_MANAGER_H

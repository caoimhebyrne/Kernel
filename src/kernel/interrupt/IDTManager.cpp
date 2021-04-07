#include "IDTManager.h"

/**
 * initializes the idt, this allows us to handle software and hardware interrupts
 * here, we are defining the exceptions that we want to catch
 *
 * at the moment, we only catch double fault (0x8)
 */
IDTDescriptor IDTManager::table[256];

/**
 * allows you to register a method that the cpu will call when your specified exception is thrown
 * this function expects that the exception that you are catching provides an error code (size_t)
 *
 * @param selector the selector of the exception
 * @param func a pointer to your function which will be invoked by the cpu
 */
void IDTManager::registerExceptionHandler(uint16_t selector, void (*func)(InterruptFrame *, size_t)) {
    IDTDescriptor *descriptor = &table[selector];
    this->initializeDescriptor((uint64_t) func, (1 << 7) | (0 << 5) | 0xF, descriptor);
}

/**
 * type_attr structure:
 *   7                           0
 * +---+---+---+---+---+---+---+---+
 * | P |  DPL  | Z |    GateType   |
 * +---+---+---+---+---+---+---+---+
 *
 * P: if this is set, you are saying that there is something in this entry
 * DPL: can users execute it? (0 if no, 3 if yes)
 * Z: most likely the storage segment. set to 0 for interrupt and trap gates
 * gate type: 0xE for interrupts and 0xF for exceptions
 * =========================================================================
 *
 * sets the information required for an idt descriptor
 *
 * @param offset the offset of the function to call
 * @param type_attr see above
 * @param desc the descriptor to write to
 */
void IDTManager::initializeDescriptor(uint64_t offset, uint16_t type_attr, struct IDTDescriptor *desc) {
    desc->offset_1 = (offset & 0xffff);
    desc->selector = 0x8;
    desc->ist = 0 & 0b11;
    desc->type_attr = type_attr;
    desc->offset_2 = (offset >> 16) & 0xffff;
    desc->offset_3 = (offset >> 32) & 0xffffffff;
    desc->zero = 0;
}

IDTDescriptor *IDTManager::getTable() {
    return table;
}

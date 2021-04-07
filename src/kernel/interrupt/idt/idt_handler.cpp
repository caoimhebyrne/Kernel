#include "idt_handler.h"
#include "../../io/io.h"

/**
 * called by the cpu when a double fault is thrown
 * a double fault is thrown when an exception is unhandled or occurs when the CPU is trying to call a handler
 */
[[noreturn]] __attribute__ ((interrupt))
void handle_double_fault(struct interrupt_frame *frame, size_t errorCode) {
    // write "df" for "double fault"
    Display::draw_string("DF");

    // print some info
    IO::print_string("a double fault has been thrown!\ncode: %u, ip:0x%u, sp:0x%u\nhalting!\n", errorCode, frame->instruction_pointer, frame->stack_pointer);

    while (true) _halt();
}

/**
 * initializes the idt, this allows us to handle software and hardware interrupts
 * here, we are defining the exceptions that we want to catch
 *
 * at the moment, we only catch double fault (0x8)
 */
IDTDescr IDTHandler::idt[256];
IDTDescr *IDTHandler::initialize() {
    // 0x8 is a double fault
    IDTDescr *page_fault_desc = &idt[0x8];
    this->initialize_descriptor(0x8, (uint64_t) &handle_double_fault, (1 << 7) | (0 << 5) | 0xF, page_fault_desc);

    return idt;
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
 * @param selector the "exception" to cache
 * @param offset the offset of the function to call
 * @param type_attr see above
 * @param desc the descriptor to write to
 */
void IDTHandler::initialize_descriptor(uint16_t selector, uint64_t offset, uint16_t type_attr, struct IDTDescr *desc) {
    desc->offset_1 = (offset & 0xffff);
    desc->selector = selector;
    desc->type_attr = type_attr;
    desc->offset_2 = (offset >> 16) & 0xffff;
    desc->offset_3 = (offset >> 32) & 0xffffffff;
}

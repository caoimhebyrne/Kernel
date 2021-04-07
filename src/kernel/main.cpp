#include "interrupt/idt/idt_handler.h"
#include "io/io.h"

__attribute__((interrupt))
void handle_general_protection_fault(interrupt_frame *frame, size_t code);

__attribute__((interrupt))
void handle_double_fault(interrupt_frame *frame, size_t code);

void setup_idt();

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    // print ready to the screen
    Display::draw_string("READY");
    IO::print_string("hello world\n");

    // setup the interrupt descriptor table
    setup_idt();

    // trigger a general protection fault, it should be caught
    // *((char *) 0xffffffffffff) = 'A';
}

/**
 * this function does all the steps required for setting up and enabling the idt
 * at the moment, the only exception that is being caught is double fault (0x8)
 */
void setup_idt() {
    IDTHandler idtHandler;
    idtHandler.register_exception_handler(0x8, handle_double_fault);
    idtHandler.register_exception_handler(0xd, handle_general_protection_fault);

    // load the interrupt descriptor table
    IDTDescr *table = idtHandler.get_idt();
    lidt((uint64_t) &table[0], sizeof(*table) * 256);
}

void handle_general_protection_fault(interrupt_frame *frame, size_t code) {
    Display::draw_string("#GP");

    IO::print_string("a general protection fault has been thrown!\ncode: %l, ip:0x%l, sp:0x%l\nhalting!\n",
                     code, frame->instruction_pointer, frame->stack_pointer);

    halt();
}

void handle_double_fault(interrupt_frame *frame, size_t code) {
    Display::draw_string("#DF");

    IO::print_string("a double fault has been thrown!\ncode: %l, ip:0x%l, sp:0x%l\nhalting!\n", code,
                     frame->instruction_pointer, frame->stack_pointer);

    halt();
}
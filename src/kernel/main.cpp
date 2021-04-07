#include "interrupt/idt/idt_handler.h"

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    // print ready to the screen
    Display::draw_string("READY");

    // setup the interrupt descriptor table
    IDTHandler idtHandler;
    IDTDescr *table = idtHandler.initialize();

    struct {
        uint16_t size;
        uint64_t addr;
    } __attribute__((packed)) idtRegister = {
        sizeof(*table) * 256,
        (uint64_t) &table[0]
    };

    // load the idt that we have defined
    asm volatile("lidt %0"::"m" (idtRegister));

    // trigger a page fault, it should be caught
    // *((char *) 0xffffffffffff) = 'A';
}

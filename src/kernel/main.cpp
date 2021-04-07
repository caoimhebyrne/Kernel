#include "interrupt/idt/idt_handler.h"
#include "io/io.h"

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    // print ready to the screen
    Display::draw_string("READY");
    IO::print_string("hello world\n");

    // setup the interrupt descriptor table
    IDTHandler idtHandler;
    IDTDescr *table = idtHandler.initialize();

    // load the idt that we have defined
    lidt((uint64_t) &table[0], sizeof(*table) * 256);

    // trigger a page fault, it should be caught
    *((char *) 0xffffffffffff) = 'A';
}

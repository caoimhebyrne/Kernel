#include "interrupt/IDTManager.h"
#include "io/IO.h"
#include "scheduler/Timer.h"
#include "interrupt/pic/PIC.h"

__attribute__((interrupt))
void handleGeneralProtectionFault(InterruptFrame *frame, size_t code);

__attribute__((interrupt))
void handleDoubleFault(InterruptFrame *frame, size_t code);

void setupIDT();

// a list of global constructors
extern "C" {
extern void (*__CTOR_LIST__)();
}

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    // loop through all global constructors and invoke them
    void (**constructor)() = &__CTOR_LIST__;
    while (*constructor) {
        (*constructor)();
        constructor++;
    }

    // setup the interrupt descriptor table
    setupIDT();

    PIC::initialize();
    PIC::PIC1_mask(0b11111110);
    PIC::PIC2_mask(0b11111111);

    Timer::initialize(100);
    sti();

    Display::drawString("Waiting 3 seconds...");
    while (Timer::ticks % 300 != 0);
    Display::drawString("Ready!");

    halt();
}

/**
 * this function does all the steps required for setting up and enabling the idt
 * at the moment, the only exception that is being caught is double fault (0x8)
 */
void setupIDT() {
    IDTManager idtManager;
    idtManager.registerExceptionHandler(0x8, handleDoubleFault);
    idtManager.registerExceptionHandler(0xd, handleGeneralProtectionFault);
    idtManager.registerExceptionHandler(PIC1 + 0, Timer::handle);

    // load the interrupt descriptor table
    IDTDescriptor *table = idtManager.getTable();
    lidt((uint64_t) &table[0], sizeof(*table) * 256);
}

void handleGeneralProtectionFault(InterruptFrame *frame, size_t code) {
    Display::drawString("#GP");

    IO::printf("a general protection fault has been thrown!\ncode: %l, ip:0x%l, sp:0x%l\nhalting!\n",
               code, frame->instructionPointer, frame->stackPointer);

    halt();
}

void handleDoubleFault(InterruptFrame *frame, size_t code) {
    Display::drawString("#DF");

    IO::printf("a double fault has been thrown!\ncode: %l, ip:0x%l, sp:0x%l\nhalting!\n", code,
               frame->instructionPointer, frame->stackPointer);

    halt();
}
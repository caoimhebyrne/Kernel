#include "interrupt/IDTManager.h"
#include "io/IO.h"
#include "scheduler/Timer.h"
#include "interrupt/pic/PIC.h"
#include "../include/multiboot2.h"

__attribute__((interrupt))
void handleGeneralProtectionFault(InterruptFrame *frame, size_t code);

__attribute__((interrupt))
void handleDoubleFault(InterruptFrame *frame, size_t code);

void setupIDT();

// information provided to us by multiboot2
extern "C" uint32_t multiboot_ptr;
extern "C" uint32_t multiboot_magic;

// a list of global constructors
extern "C" {
extern void (*__CTOR_LIST__)();
}

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    IO::printf("Welcome to monke kernel iso 15 (real)...\n");

    if (multiboot_magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        Display::drawString("INVALID MULTIBOOT2 MAGIC!");
        IO::printf("Invalid Multiboot2 magic! Expected: 0x%l, received: 0x%l...\nhalting!\n",
                   MULTIBOOT2_BOOTLOADER_MAGIC, multiboot_magic);
        halt();
        return;
    }

    IO::printf(
            "========== Multiboot info dump ==========\n- Received magic: 0x%l\n- Received pointer: 0x%l\n=========================================\n",
            multiboot_magic, multiboot_ptr);

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
    Timer::sleep(3000);
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
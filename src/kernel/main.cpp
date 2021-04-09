#include "interrupt/idt/manager/IDTManager.h"
#include "io/IO.h"
#include "scheduler/Timer.h"
#include "interrupt/pic/PICManager.h"
#include "../include/multiboot2.h"
#include "interrupt/idt/handler/IDTHandler.h"

// information provided to us by multiboot2
extern "C" uint32_t multiboot_ptr;
extern "C" uint32_t multiboot_magic;

// a list of global constructors
extern "C" {
extern void (*__CTOR_LIST__)(); // NOLINT(bugprone-reserved-identifier)
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
    IDTHandler::initialize();

    PICManager::initialize();
    PICManager::PIC1_mask(0b11111110);
    PICManager::PIC2_mask(0b11111111);

    Timer::initialize(100);
    sti();

    Display::drawString("Waiting 3 seconds...");
    Timer::sleep(3000);
    Display::drawString("Ready!");

    halt();
}

#include "interrupt/idt/manager/IDTManager.h"
#include "io/IO.h"
#include "scheduler/timer/Timer.h"
#include "interrupt/pic/PICManager.h"
#include "interrupt/idt/handler/IDTHandler.h"
#include "multiboot/MultibootParser.h"
#include "scheduler/task/Task.h"

// information provided to us by multiboot2
extern "C" uint32_t multiboot_ptr;
extern "C" uint32_t multiboot_magic;

// a list of global constructors
extern "C" {
extern void (*__CTOR_LIST__)(); // NOLINT(bugprone-reserved-identifier)
}

static void test() {
    IO::printf("We are at test!\n");
}

/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    auto valid = MultibootParser::verify(multiboot_magic, multiboot_ptr);
    if (!valid) {
        Display::drawString("Invalid multiboot2 information provided!");
        IO::printf("Invalid multiboot2 information provided! Magic: 0x%l, pointer: 0x%l...\nhalting!\n",
                   multiboot_magic, multiboot_ptr);
        halt();
        return;
    }

    auto multibootInformation = MultibootParser::parse(multiboot_ptr);
    IO::printf(
            "========== Multiboot info dump ==========\n"
            "- Received magic: 0x%l\n"
            "- Received pointer: 0x%l\n"
            "- MBI size: 0x%l\n"
            "- Boot loader name: %s\n"
            "- Lower memory: %lKB\n"
            "- Upper memory: %lKB\n"
            "=========================================\n",
            multiboot_magic, multiboot_ptr, *(unsigned *) multiboot_ptr, multibootInformation.bootLoaderName,
            multibootInformation.lowerMemory, multibootInformation.upperMemory);

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

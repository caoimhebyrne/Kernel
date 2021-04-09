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
    } else if (multiboot_ptr & 7) {
        Display::drawString("Unaligned MBI!");
        IO::printf("Unaligned MBI received! Address: 0x%l...\nhalting!\n",
                   multiboot_ptr);
        halt();
        return;
    }

    auto size = *(unsigned *) multiboot_ptr;
    IO::printf(
            "========== Multiboot info dump ==========\n- Received magic: 0x%l\n- Received pointer: 0x%l\n- MBI size: 0x%l\n",
            multiboot_magic, multiboot_ptr, size);

    // read multiboot2 information passed to us via the pointer
    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag *) (multiboot_ptr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag *) ((uint8_t *) tag
                                         + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_CMDLINE: {
                char *string = ((struct multiboot_tag_string *) tag)->string;
                // IO::printf("- Command line: %s\n", string);
                break;
            }
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
                IO::printf("- Boot loader name: %s\n",
                           ((struct multiboot_tag_string *) tag)->string);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                IO::printf("- Lower memory: %iKB\n- Upper memory: %iKB\n",
                           ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
                           ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                break;
            case MULTIBOOT_TAG_TYPE_BOOTDEV:
                IO::printf("- Boot device: 0x%l, %l, %l\n",
                           ((struct multiboot_tag_bootdev *) tag)->biosdev,
                           ((struct multiboot_tag_bootdev *) tag)->slice,
                           ((struct multiboot_tag_bootdev *) tag)->part);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: {
                multiboot_memory_map_t *mmap;

                IO::printf("- Memory map:\n");

                for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
                     (multiboot_uint8_t *) mmap
                     < (multiboot_uint8_t *) tag + tag->size;
                     mmap = (multiboot_memory_map_t *)
                             ((unsigned long) mmap
                              + ((struct multiboot_tag_mmap *) tag)->entry_size))
                    IO::printf("  - base address: 0x%l%l,"
                               " length: 0x%l%l, type: 0x%l\n",
                               (unsigned) (mmap->addr >> 32),
                               (unsigned) (mmap->addr & 0xffffffff),
                               (unsigned) (mmap->len >> 32),
                               (unsigned) (mmap->len & 0xffffffff),
                               (unsigned) mmap->type);
            }
                break;
        }
    }

    IO::printf("=========================================\n");


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

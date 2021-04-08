#include "PICManager.h"
#include "../../asm/asm.h"
#include "../../io/IO.h"

void PICManager::initialize() {
    // save masks before modification
    uint8_t pic1_data = inb(PIC1_DATA);
    uint8_t pic2_data = inb(PIC2_DATA);

    // start the initialization sequence (cascade mode)
    IO::pwrite(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    IO::pwrite(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // write offsets
    IO::pwrite(PIC1_DATA, PIC1);
    IO::pwrite(PIC2_DATA, PIC2);

    // tell master pic that there is a slave pic at irq2
    IO::pwrite(PIC1_DATA, 4);

    // tell slave pic its cascade identity
    IO::pwrite(PIC2_DATA, 2);

    // use 8086 / 68 mode
    IO::pwrite(PIC1_DATA, ICW4_8086);
    IO::pwrite(PIC2_DATA, ICW4_8086);

    // restore saved masks
    IO::pwrite(PIC1_DATA, pic1_data);
    IO::pwrite(PIC2_DATA, pic2_data);
}

void PICManager::PIC1_mask(int mask) {
    IO::pwrite(PIC1_DATA, mask);
}

void PICManager::PIC2_mask(int mask) {
    IO::pwrite(PIC2_DATA, mask);
}

void PICManager::sendEOI() {
    IO::pwrite(PIC1_COMMAND, 0x20);
}

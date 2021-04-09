#include "Panic.h"
#include "../asm/asm.h"
#include "../interrupt/pic/PICManager.h"
#include "../io/IO.h"

void Panic::invokePanic(const char *reason, const char *details, ...) {
    va_list args;
    va_start(args, details);

    // disable interrupts and disable PIC1 & PIC2
    cli();
    PICManager::PIC1_mask(0b11111111);
    PICManager::PIC2_mask(0b11111111);

    IO::printf("kernel panic ;(\nreason:\n  %s\ndetails:\n%s\nstack: \nhalting...\n", reason, details, args);

    va_end(args);
    while (true) halt();
}

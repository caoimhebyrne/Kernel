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

    IO::printf("kernel panic ;(\nreason:\n  %s\ndetails:\n  %s\nstack:\n", reason, details, args);

    // print stack trace
    struct StackFrame *frame;
    asm volatile("mov %%rbp, %0" : "=r" (frame));
    for (auto i = 0; frame && i < 5; ++i) {
        IO::printf("  0x%l\n", frame->rip);
        frame = frame->rsp;
    }

    IO::printf("\nhalting...");

    va_end(args);
    while (true) halt();
}

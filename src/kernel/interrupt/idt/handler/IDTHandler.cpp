#include "IDTHandler.h"
#include "../manager/IDTManager.h"
#include "../../../scheduler/Timer.h"
#include "../../pic/PICManager.h"
#include "../../../io/IO.h"

void handleGeneralProtectionFault(InterruptFrame *frame, size_t code) {
    IDTHandler::handleFault("#GP", "general protection fault", frame, code);
}

void handleDoubleFault(InterruptFrame *frame, size_t code) {
    IDTHandler::handleFault("#DF", "double fault", frame, code);
}

void IDTHandler::handleFault(const char *displayCode, const char *name, InterruptFrame *frame, size_t code) {
    Display::drawString(displayCode);

    IO::printf("a %s has been thrown!\ncode: %l, ip:0x%l, sp:0x%l\nhalting!\n", name, code,
               frame->instructionPointer, frame->stackPointer);

    halt();
}

void IDTHandler::initialize() {
    IDTManager idtManager;
    idtManager.registerExceptionHandler(0x8, handleDoubleFault);
    idtManager.registerExceptionHandler(0xd, handleGeneralProtectionFault);
    idtManager.registerExceptionHandler(PIC1 + 0, Timer::handle);

    // load the interrupt descriptor table
    auto *table = idtManager.getTable();
    lidt((uint64_t) &table[0], sizeof(*table) * 256);
}


#include "IDTHandler.h"
#include "../manager/IDTManager.h"
#include "../../../scheduler/timer/Timer.h"
#include "../../pic/PICManager.h"
#include "../../../io/IO.h"
#include "../../../panic/Panic.h"

void handleGeneralProtectionFault(InterruptFrame *frame, size_t code) {
    IDTHandler::handleFault("#GP", "general protection fault", frame, code);
}

void handleDoubleFault(InterruptFrame *frame, size_t code) {
    IDTHandler::handleFault("#DF", "double fault", frame, code);
}

void handlePageFault(InterruptFrame *frame, size_t code) {
    IDTHandler::handleFault("#PF", "page fault", frame, code);
}

void IDTHandler::handleFault(const char *displayCode, const char *name, InterruptFrame *frame, size_t code) {
    Display::drawString(displayCode);
    Panic::invokePanic(name, "code: %l\n  ip: 0x%l\n  cs: 0x%l\n  flags: 0x%l\n  sp: 0x%l\n  ss: 0x%l", code,
                       frame->instructionPointer, frame->codeSegment, frame->flags, frame->stackPointer,
                       frame->stackSegment);
}

void IDTHandler::initialize() {
    IDTManager idtManager;
    idtManager.registerExceptionHandler(0x8, handleDoubleFault);
    idtManager.registerExceptionHandler(0xd, handleGeneralProtectionFault);
    idtManager.registerExceptionHandler(0xe, handlePageFault);
    idtManager.registerExceptionHandler(PIC1 + 0, Timer::handle);

    // load the interrupt descriptor table
    auto *table = idtManager.getTable();
    lidt((uint64_t) &table[0], sizeof(*table) * 256);
}


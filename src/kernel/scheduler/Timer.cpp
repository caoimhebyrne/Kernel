#include "Timer.h"
#include "../io/IO.h"
#include "../interrupt/pic/PIC.h"

int Timer::ticks;

void Timer::initialize(int freq) {
    int divisor = 1193180 / freq;

    // to set the rate at which channel 0 fires off the IRQ0, we write to the timer chanels
    // the timer will divide it's input clock of 1.19mhz by the number in the data register
    // to figure out how many times per second to fire that signal.
    IO::pwrite(CMD_REG_PORT, 0x36); // set our command byte, 0x36
    IO::pwrite(DATA_REG_1, divisor & 0xFF); // set low byte of the divisor
    IO::pwrite(DATA_REG_1, divisor >> 8); // set high byte of the divisor
}

void Timer::handle(InterruptFrame *frame) {
    ticks++;
    PIC::sendEOI();
}

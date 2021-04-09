#include "Timer.h"
#include "../../io/IO.h"
#include "../../interrupt/pic/PICManager.h"

int Timer::ticks;

void Timer::initialize(int freq) {
    int divisor = 1193180 / freq;

    // to set the rate at which channel 0 fires off the IRQ0, we write to the timer channels
    // the timer will divide it's input clock of 1.19mhz by the number in the data register
    // to figure out how many times per second to fire that signal.
    IO::pwrite(CMD_REG_PORT, 0x36); // set our command byte, 0x36
    IO::pwrite(DATA_REG_1, divisor & 0xFF); // set low byte of the divisor
    IO::pwrite(DATA_REG_1, divisor >> 8); // set high byte of the divisor
}

void Timer::handle(InterruptFrame *frame) {
    ticks++;
    PICManager::sendEOI();
}

/**
 * halts the cpu for a certain amount of milliseconds
 * this function assumes that the timer is running at 100hz
 * @param ms the amount of milliseconds to sleep for
 */
void Timer::sleep(int ms) {
    while (ticks % (ms / 10) != 0);
}
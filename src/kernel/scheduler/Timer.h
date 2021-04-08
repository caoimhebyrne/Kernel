#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#define CMD_REG_PORT 0x43
#define DATA_REG_1 0x40

#include "../interrupt/types/InterruptFrame.h"

class Timer {
private:
    static int ticks;

public:
    static void initialize(int freq);

    static void sleep(int ms);

    __attribute__((interrupt))
    static void handle(InterruptFrame *frame);
};


#endif //KERNEL_TIMER_H

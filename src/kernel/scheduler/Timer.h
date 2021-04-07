#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#define CMD_REG_PORT 0x43
#define DATA_REG_1 0x40

#include "../interrupt/types/InterruptFrame.h"

class Timer {
public:
    static int ticks;

    static void initialize(int freq);

    __attribute__((interrupt))
    static void handle(InterruptFrame *frame);
};


#endif //KERNEL_TIMER_H

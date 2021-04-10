#ifndef KERNEL_SCHEDULER_H
#define KERNEL_SCHEDULER_H


#include "task/Task.h"

class Scheduler {
public:
    static void initialize();

    static void addTask(void(*func)());

    static void executeNextTask();

    static void switchToCurrent();

private:
    static Task nextTask;
    static Task currentTask;
    static size_t newTaskStack[16];
};


#endif //KERNEL_SCHEDULER_H

#include "Scheduler.h"
#include "../panic/Panic.h"

Task Scheduler::currentTask;
Task Scheduler::nextTask;
size_t Scheduler::newTaskStack[16];

void Scheduler::initialize() {
    switchTask(&currentTask, &currentTask);
}

void Scheduler::addTask(void (*func)()) {
    newTaskStack[15] = (size_t) &func;

    nextTask.rsp = (size_t) &newTaskStack[15];
    nextTask.rflags = 6;
    nextTask.cr0 = currentTask.cr0;
    nextTask.cr2 = currentTask.cr2;
    nextTask.cr3 = currentTask.cr3;
    nextTask.cr4 = currentTask.cr4;
    nextTask.cr8 = currentTask.cr8;
    nextTask.ds = 0;
    nextTask.ss = 0;
    nextTask.fs = 0;
    nextTask.gs = 0;
}

void Scheduler::executeNextTask() {
    if (nextTask.rsp == 0) return;
    switchTask(&nextTask, &currentTask);
}

void Scheduler::switchToCurrent() {
    if (nextTask.rsp == 0) {
        Panic::invokePanic("bad stack", "Scheduler::switchToCurrent encountered a task with a bad stack!");
    }
    switchTask(&currentTask, &nextTask);
}

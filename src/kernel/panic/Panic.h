#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include <stdarg.h>
#include <stdint.h>

struct StackFrame {
    struct StackFrame *ebp;
    uint32_t eip;
};

class Panic {
public:
    static void invokePanic(const char *reason, const char *details, ...);

private:
    static StackFrame *getStackTrace(int frames);
};


#endif //KERNEL_PANIC_H

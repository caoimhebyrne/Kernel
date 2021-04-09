#ifndef KERNEL_PANIC_H
#define KERNEL_PANIC_H

#include <stdarg.h>


class Panic {
public:
    static void invokePanic(const char *reason, const char *details, ...);
};


#endif //KERNEL_PANIC_H

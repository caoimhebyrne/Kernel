#ifndef KERNEL_IDTHANDLER_H
#define KERNEL_IDTHANDLER_H

#include "../types/InterruptFrame.h"

class IDTHandler {
public:
    static void initialize();

    static void handleFault(const char *displayCode, const char *name, InterruptFrame *frame, size_t code);
};


#endif //KERNEL_IDTHANDLER_H

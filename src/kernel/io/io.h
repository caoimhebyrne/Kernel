#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#define COM1 0x3F8

#include "../asm/asm.h"

class IO {
public:
    static void print_string(const char *string);
    static void print_char(char c);
};

#endif //KERNEL_IO_H

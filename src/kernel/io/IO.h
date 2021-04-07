#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

#define COM1 0x3F8

class IO {
public:
    static void printf(const char *string, ...);

    static void printc(char c);

    static void pwrite(uint16_t port, uint8_t value);
};

#endif //KERNEL_IO_H

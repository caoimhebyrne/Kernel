#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#define COM1 0x3F8

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

class IO {
public:
    static void print_string(const char *string);
    static void print_char(char c);
};

/**
 * sends a value to an io location
 * @param port the location to send the value to
 * @param value the value to send to the location
 */
[[maybe_unused]] static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1"::"a"(value), "Nd"(port));
}

/**
 * retrieves a value from an io location
 * @param port the port to retrieve the value from
 * @return the value
 */
[[maybe_unused]] static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0": "=a"(ret): "Nd"(port));
    return ret;
}

#endif //KERNEL_IO_H

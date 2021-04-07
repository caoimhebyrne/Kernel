#ifndef KERNEL_ASM_H
#define KERNEL_ASM_H

#include <stdint.h> // NOLINT(modernize-deprecated-headers)

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

/**
 * tells the cpu to not run any more instructions
 */
[[maybe_unused]] static inline void halt() {
    while (true) asm volatile("hlt");
}

/**
 * defines a new interrupt table (idt)
 * @param base the pointer of the first idt entry in your array
 * @param size the size of your idt table
 */
[[maybe_unused]] static inline void lidt(uint64_t address, uint16_t size) {
    struct {
        uint16_t size;
        uint64_t addr;
    } __attribute__((packed)) idtRegister = {size, address};

    asm volatile ("lidt %0" : : "m"(idtRegister));
}

#endif //KERNEL_ASM_H

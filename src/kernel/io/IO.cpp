#include <stdarg.h> // NOLINT(modernize-deprecated-headers)

#include "IO.h"
#include "../asm/asm.h"
#include "util/integer_utils.h"

void IO::pwrite(uint16_t port, uint8_t value) {
    // wait until the output buffer is flushed
    while (!(inb(port + 5) & 0b100000));

    // write our character to the buffer
    outb(port, value);
}

void IO::printc(char c) {
    pwrite(COM1, c);
}

void IO::printf(const char *string, ...) {
    va_list args;
    va_start(args, string);

    // iterate over all characters of the string until null terminator
    for (int i = 0; string[i] != '\0'; i++) {
        char c = string[i];

        // check if the char is %
        if (c == '%') {
            // get the next character
            i++;
            char nextC = string[i];

            // parse the args
            switch (nextC) {
                case 's': {
                    // string
                    const char *arg = va_arg(args, const char *);
                    printf(arg);
                    break;
                }
                case 'c': {
                    // unsigned char
                    uint8_t arg = va_arg(args, uint32_t) & 0xffff;
                    printf(toString(arg));
                    break;
                }
                case 'u': {
                    // unsigned short
                    uint16_t arg = va_arg(args, uint32_t) & 0xffff;
                    printf(toString(arg));
                    break;
                }
                case 'i': {
                    // unsigned int
                    uint32_t arg = va_arg(args, uint32_t);
                    printf(toString(arg));
                    break;
                }
                case 'l': {
                    // unsigned long
                    uint64_t arg = va_arg(args, uint64_t);
                    printf(toString(arg));
                    break;
                }
                default: {
                    // print out the original sequence of characters
                    printc(c);
                    printc(nextC);
                    break;
                }
            }
        } else {
            // print the character to COM1
            printc(c);
        }
    }

    va_end(args);
}

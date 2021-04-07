#include <stdarg.h> // NOLINT(modernize-deprecated-headers)

#include "io.h"
#include "../asm/asm.h"
#include "util/integer_utils.h"

void IO::print_char(char c) {
    // wait until the output buffer is flushed
    while (!(inb(COM1 + 5) & 0b100000));

    // write our character to the buffer
    outb(COM1, c);
}

void IO::print_string(const char *string, ...) {
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
                    print_string(arg);
                    break;
                }
                case 'c': {
                    // unsigned char
                    uint8_t arg = va_arg(args, uint32_t) & 0xffff;
                    print_string(toString(arg));
                    break;
                }
                case 'u': {
                    // unsigned short
                    uint16_t arg = va_arg(args, uint32_t) & 0xffff;
                    print_string(toString(arg));
                    break;
                }
                case 'i': {
                    // unsigned int
                    uint32_t arg = va_arg(args, uint32_t);
                    print_string(toString(arg));
                    break;
                }
                case 'l': {
                    // unsigned long
                    uint64_t arg = va_arg(args, uint64_t);
                    print_string(toString(arg));
                    break;
                }
                default: {
                    // print out the original sequence of characters
                    print_char(c);
                    print_char(nextC);
                    break;
                }
            }
        } else {
            // print the character to COM1
            print_char(c);
        }
    }

    va_end(args);
}

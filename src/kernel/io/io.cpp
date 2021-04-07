#include "io.h"

void IO::print_char(char c) {
    // wait until the output buffer is flushed
    while (!(inb(COM1 + 5) & 0b100000));

    // write our character to the buffer
    outb(COM1, c);
}

void IO::print_string(const char *string) {
    // iterate over all characters of the string until null terminator
    for (int i = 0; string[i] != '\0'; i++) {
        // print the character to COM1
        print_char(string[i]);
    }
}

#ifndef KERNEL_INTEGER_UTILS_H
#define KERNEL_INTEGER_UTILS_H

#include <stddef.h> // NOLINT(modernize-deprecated-headers)

template<class T>
static const char *toString(T value) {
    static char buf[(sizeof(value) << 1) + 2];
    size_t idx = (sizeof(value) << 1) + 1;
    buf[idx] = '\0';

    bool negative = value < 0;
    if (negative) value = -value;

    while (value >= 0x16) {
        buf[--idx] = "0123456789ABCDEF"[value & 0xF];
        value >>= 4;
    }

    buf[--idx] = "0123456789ABCDEF"[value & 0xF];
    if (negative) buf[--idx] = '-';
    return &buf[idx];
}

#endif //KERNEL_INTEGER_UTILS_H

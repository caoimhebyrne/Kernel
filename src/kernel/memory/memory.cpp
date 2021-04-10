#include <stddef.h>

/**
 * fill a certain amount of bytes of the pointer with a certain byte
 * @param pointer the area to fill the bytes in
 * @param byte the byte to use
 * @param count the amount of times to fill
 */
extern "C" [[maybe_unused]] void *memset(void *pointer, int byte, size_t count) {
    auto *castedPointer = (char *) pointer;
    for (size_t i = 0; i < count; i++) {
        castedPointer[i] = (char) (byte & 0xff);
    }

    return pointer;
}

/**
 * copies a certain amount of bytes from one location to another
 *
 * @param destination the area to copy bytes from
 * @param source the area to move the bytes to
 * @param count the amount of times to iterate this
 */
extern "C" [[maybe_unused]] void *memcpy(void *destination, void *source, size_t count) {
    auto *castedDestination = (char *) destination;
    auto *castedSource = (char *) source;

    for (size_t i = 0; i < count; i++) {
        castedDestination[i] = castedSource[i];
    }

    return destination;
}

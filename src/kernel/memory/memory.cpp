#include <stddef.h>

/**
 * fill a certain amount of bytes of the pointer with a certain byte
 * @param pointer the area to fill the bytes in
 * @param byte the byte to use
 * @param count the amount of times to fill
 */
extern "C" [[maybe_unused]] void *memset(void *pointer, int byte, size_t count) {
    auto *castedPointer = (char *) pointer;
    for (auto i = 0; i < count; i++) {
        castedPointer[i] = (char) (byte & 0xff);
    }

    return pointer;
}

#ifndef KERNEL_MULTIBOOTPARSER_H
#define KERNEL_MULTIBOOTPARSER_H


#include <stdint.h> // NOLINT(modernize-deprecated-headers)

struct MultibootBootDeviceTag {
    uint32_t biosDevice; // 0x00 for the first floppy disk or 0x80 for the first hard disk
    uint32_t slice;
    uint32_t part;
};

struct MultibootMemoryMap {
    uint32_t address;
    uint32_t length;
    uint32_t type;
};

struct MultibootTagsInformation {
    const char *bootLoaderName;
    uint32_t lowerMemory;
    uint64_t upperMemory;
    MultibootBootDeviceTag bootDevice;
    MultibootMemoryMap memoryMap[10]; // FIXME: Remove this once memory management is added
};


class MultibootParser {
public:
    static bool verify(uint32_t magic, uint32_t pointer);

    static MultibootTagsInformation parse(uint32_t pointer);
};


#endif //KERNEL_MULTIBOOTPARSER_H

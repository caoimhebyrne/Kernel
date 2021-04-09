#include "MultibootParser.h"
#include "../../include/multiboot2.h"

MultibootTagsInformation MultibootParser::parse(unsigned int pointer) {
    // read multiboot2 information passed to us via the pointer
    MultibootTagsInformation tagsInformation = {};
    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag *) (uint64_t) (pointer + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END; tag = (struct multiboot_tag *) ((uint8_t *) tag +
                                                                              ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME: {
                auto stringTag = ((struct multiboot_tag_string *) tag);
                tagsInformation.bootLoaderName = (const char *) stringTag->string;
                break;
            }
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: {
                auto memoryInfo = (struct multiboot_tag_basic_meminfo *) tag;
                tagsInformation.lowerMemory = memoryInfo->mem_lower;
                tagsInformation.upperMemory = memoryInfo->mem_upper;
                break;
            }
            case MULTIBOOT_TAG_TYPE_BOOTDEV: {
                auto bootDeviceInfo = (struct multiboot_tag_bootdev *) tag;
                tagsInformation.bootDevice.biosDevice = bootDeviceInfo->biosdev;
                tagsInformation.bootDevice.slice = bootDeviceInfo->slice;
                tagsInformation.bootDevice.part = bootDeviceInfo->part;
                break;
            }
            case MULTIBOOT_TAG_TYPE_MMAP: {
                auto memoryMap = tagsInformation.memoryMap;
                auto memoryTag = (struct multiboot_tag_mmap *) tag;

                auto counter = 0;
                multiboot_memory_map_t *mmap;
                for (mmap = memoryTag->entries; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag + tag->size;
                     mmap = (multiboot_memory_map_t *)
                             ((unsigned long) mmap + memoryTag->entry_size)) {
                    memoryMap[counter].address = mmap->addr;
                    memoryMap[counter].length = mmap->len;
                    memoryMap[counter].type = mmap->type;

                    counter++;
                }
                break;
            }
        }
    }

    return tagsInformation;
}

bool MultibootParser::verify(uint32_t magic, uint32_t pointer) {
    return (magic == MULTIBOOT2_BOOTLOADER_MAGIC) && !(pointer & 7);
}


#include <stdint.h> // NOLINT(modernize-deprecated-headers)

#ifndef KERNEL_IDT_DESCRIPTOR_H
#define KERNEL_IDT_DESCRIPTOR_H

struct IDTDescriptor {
    uint16_t offset1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t typeAttr; // type and attributes
    uint16_t offset2; // offset bits 16..31
    uint32_t offset3; // offset bits 32..63
    uint32_t zero;     // reserved
};

#endif //KERNEL_IDT_DESCRIPTOR_H
#include "types/idt_descriptor.h"
#include "types/interrupt_frame.h"
#include "../../display/display.h"

class IDTHandler {
public:
    IDTDescr *initialize();

    void initialize_descriptor(uint16_t selector, uint64_t offset, uint16_t type_attr, struct IDTDescr *desc);

private:
    static IDTDescr idt[256];
};

#ifndef KERNEL_PICMANAGER_H
#define KERNEL_PICMANAGER_H

#define PIC1 0x20
#define PIC2 PIC1 + 8
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

class PICManager {
public:
    static void initialize();

    static void PIC1_mask(int mask);

    static void PIC2_mask(int mask);

    static void sendEOI();
};


#endif //KERNEL_PICMANAGER_H

#ifndef KERNEL_IO_H
#define KERNEL_IO_H
#define COM1 0x3F8

class IO {
public:
    static void printf(const char *string, ...);

    static void printc(char c);
};

#endif //KERNEL_IO_H

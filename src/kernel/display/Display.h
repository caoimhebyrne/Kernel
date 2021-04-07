#ifndef KERNEL_DISPLAY_H
#define KERNEL_DISPLAY_H

class Display {
public:
    static void drawString(const char *string);

private:
    static const int vramStart = 0xb8000;
    static const int vramEnd = 0xb8fa0;

    static void clearDisplay();
};

#endif //KERNEL_DISPLAY_H

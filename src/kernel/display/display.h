#ifndef KERNEL_DISPLAY_H
#define KERNEL_DISPLAY_H

class Display {
public:
    static void draw_string(const char *string);
private:
    static const int vram_start = 0xb8000;
    static const int vram_end = 0xb8fa0;
    static void clear_display();
};

#endif //KERNEL_DISPLAY_H

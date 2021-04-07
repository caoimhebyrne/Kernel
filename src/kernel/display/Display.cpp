#include "Display.h"

void Display::drawString(const char *string) {
    // before we draw a new string, we need to clear the previous one
    clearDisplay();

    // while i is smaller than or equal tos the size of the string
    for (int i = 0; string[i] != '\0'; i++) {
        // write to the text bit of this memory address
        *((char *) vram_start + i * 2) = string[i];
    }
}

void Display::clearDisplay() {
    // loop between the range of vram_start and vram_end
    for (int i = 0; i + vram_start <= vram_end; i++)
        // clear the text bit of this memory address
        // for example, when i = 1, the address is 0xb8002
        *((char *) vram_start + i * 2) = 0;
}

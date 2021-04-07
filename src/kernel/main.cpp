/*
 * This is the entrypoint of our kernel
 * It is called from boot/main.s when we have switched into 64 bit mode
 */
extern "C" __attribute__((unused)) void kernel_main() {
    *((char *) 0xb8000) = 'M';
    *((char *) 0xb8002) = 'O';
    *((char *) 0xb8004) = 'N';
    *((char *) 0xb8006) = 'K';
    *((char *) 0xb8008) = 'E';
    *((char *) 0xb800A) = 'Y';
}

// this is the entry point called from our assembly
// here is where the swagness will happen :monkeypog:
extern "C" __attribute__((unused)) void kernel_main() {
    // 0xb8000 is the framebuffer
    // HOLY SHIT ITS A MONKEY ON THE SCREEN WHAT THE FUCK???
    *((char *) 0xb8000) = 'M';
    *((char *) 0xb8002) = 'O';
    *((char *) 0xb8004) = 'N';
    *((char *) 0xb8006) = 'K';
    *((char *) 0xb8008) = 'E';
    *((char *) 0xb800A) = 'Y';
}

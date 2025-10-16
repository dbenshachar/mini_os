/*
 * Minimal kernel implementation
 * This file is compiled to a freestanding kernel binary for QEMU
 * Execute by running
 * >>> chmod +x run.bash && ./run.bash 
 */
void kmain(void) {
    volatile unsigned int * const UART0DR = (unsigned int *)0x09000000;
    const char *str = "Kernel is running!\n";
    while (*str) {
        *UART0DR = (unsigned int)(*str);
        str++;
    }
    while (1) { }
}

void _start(void) {
    kmain();
    while (1) { }
}

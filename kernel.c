#include <stdint.h>

#define UART0_BASE 0x09000000UL
#define UARTDR (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define TXFF (1u<<5)
#define RXFE (1u<<4)

static void putc(char c){ while (UARTFR & TXFF) {} UARTDR = (unsigned)c; }
static void puts(const char*s){ while(*s) putc(*s++); }
static int  getc(void){ while (UARTFR & RXFE) {} return UARTDR & 0xFF; }

/*Execute by running:
chmod +x run.bash && ./run.bash
*/
void kmain(void){
    puts("Kernel is running!\n> ");
    for(;;){
        int c = getc();
        if (c== '\n') {
            puts("\n> ");
            continue;
        }
        if (c =='\r'){
            puts("\n> ");
            continue;
        } 
        if (c ==0x7F || c =='\b'){
            puts("\b \b");
            continue;
        }
        putc((char)c);
    }
}
#include <stdint.h>
#include "commands.h"

#define UART0_BASE 0x09000000UL
#define UARTDR (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define TXFF (1u<<5)
#define RXFE (1u<<4)

static void putc(char c){ while (UARTFR & TXFF) {} UARTDR = (unsigned)c; }
static void puts(const char*s){ while(*s) putc(*s++); }
static int  getc(void){ while (UARTFR & RXFE) {} return UARTDR & 0xFF; }

int done = 0;
char cmdbuf[256];
uint8_t cmdlen = 0;

void new_line() {
    cmdbuf[0] = '\0';
    cmdlen = 0;
}

void backspace() {
    cmdbuf[cmdlen] = '\0';
    cmdlen = (cmdlen > 0) ? cmdlen - 1 : 0;
}

/*Execute by running:
chmod +x run.bash && ./run.bash
*/
void kmain(void){
    puts("\n> ");
    
    while (1)
    {
        int c = getc();
        if (c == '\n' || c == '\r') {
            done = execute(cmdbuf);
            if (done) {puts("\nTried to quit!");}
            new_line();
            puts("\n> ");
            continue;
        }
        if (c ==0x7F || c =='\b') {
            backspace();
            puts("\b \b");
            continue;
        }
        cmdbuf[cmdlen] = c;
        cmdlen++;
        putc((char)c);
    }

    puts("\nTried to quit\n> ");
}
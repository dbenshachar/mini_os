#include <stdint.h>
#include "commands.h"

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
}
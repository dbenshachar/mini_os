#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include "files.h"

#define MAX_PARAMS 32
#define MAX_PARAM_LENGTH 16

#define UART0_BASE 0x09000000UL
#define UARTDR (*(volatile unsigned int *)(UART0_BASE + 0x00))
#define UARTFR (*(volatile unsigned int *)(UART0_BASE + 0x18))
#define TXFF (1u<<5)
#define RXFE (1u<<4)

static void putc(char c){ while (UARTFR & TXFF) {} UARTDR = (unsigned)c; }
static void puts(const char*s){ while(*s) putc(*s++); }
static int  getc(void){ while (UARTFR & RXFE) {} return UARTDR & 0xFF; }

int echo(char *string) {
    puts("\n");
    puts(string);
    return 1;
}

int quit() {
    return 0;
}

uint8_t deserialize_params(char *cmd, char **params, uint8_t max_params) {
    uint8_t count = 0;
    char *p = cmd;

    while (*p != '\0') {
        while (*p == ' ') p++;
        if (*p == '\0') break;

        if (count >= max_params) break;
        params[count++] = p;

        while (*p != '\0' && *p != ' ') p++;
        if (*p == '\0') break;

        *p = '\0';
        p++;
    }
    return count;
}

int execute(char* cmd) {
    char *params[MAX_PARAMS];
    int param_count = deserialize_params(cmd, params, MAX_PARAMS);
    if (param_count == 0) {return 0;}

    const char *exec_command = params[0];
    if (str_eq(exec_command, "quit")) {return quit();}
    if (str_eq(exec_command, "echo")) {return echo(params[1]);}
    if (str_eq(exec_command, "finit")) {return fs_init();}
    if (str_eq(exec_command, "freturn")) {fs_return(); puts("\n"); puts(target_folder->name);return 1;}
    if (str_eq(exec_command, "fdir")) {puts("\n"); puts(target_folder->name);return 1;}
    return 0;
}

#endif
#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_PARAMS 32
#define MAX_PARAM_LENGTH 16

#include <stdint.h>

void echo() {

}

int quit() {
    return 1;
}

void display() {

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

int str_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b)
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

int execute(char* cmd) {
    char *params[MAX_PARAMS];
    int param_count = deserialize_params(cmd, params, MAX_PARAMS);
    if (param_count == 0) {return 0;}

    const char *exec_command = params[0];
    if (str_eq(exec_command, "quit")) {return quit();}
    return 0;
}

#endif
#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_PARAMS 32
#define MAX_PARAM_LENGTH 16

void echo() {

}

void quit() {

}

void display() {

}

int deserialize_params(char *cmd, char **params, int max_params) {
    int count = 0;
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
    int param_count = deserialize_params(cmd, params, 0);
    return 0;
}

#endif
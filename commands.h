#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include "files.h"

#define MAX_PARAMS 32
#define MAX_PARAM_LENGTH 16

int echo(char *string) {
    puts("\n");
    puts(string);
    return 1;
}

int quit() {
    return 0;
}

int fmake(char* type, char* name) {
    if (str_eq(type, "folder")) {
        return fs_mkdir(name, target_folder);
    }
    if (str_eq(type, "file")) {
        return 1;
    }
    puts("\nInvalid make type");
    return 1;
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
    if (str_eq(exec_command, "fwipe")) {fs_wipe(target_folder); puts("\n"); puts("wiped "); puts(target_folder->name); return 1;}
    if (str_eq(exec_command, "flist")) {puts("\n"); fs_ls(target_folder); return 1;}
    if (str_eq(exec_command, "fmake")) {fmake(params[1], params[2]); return 1;}
    if (str_eq(exec_command, "fchange")) {target_folder, fs_cd(target_folder, params[1]); puts("\n"); puts(target_folder->name); return 1;}
    puts("\nInvalid command!");
    return 1;
}

#endif
#ifndef FILES_H
#define FILES_H

#define MAX_NAME_LENGTH 32
#define MAX_ITEMS 64

#include <stdint.h>

typedef struct file file;
typedef struct folder folder;

struct file {
    char name[MAX_NAME_LENGTH];
    uint32_t start_block;
    uint16_t t_size;
};

struct folder {
    char name[MAX_NAME_LENGTH];
    file files[MAX_ITEMS];
    folder* folders[MAX_ITEMS];
};

static folder root_folder;
static folder* target_folder;

int name_set(const char *src, char dst[MAX_NAME_LENGTH]) {
    int i = 0;
    if (!src || !dst) return 0;
    while (src[i] != '\0') {
        if (i >= MAX_NAME_LENGTH-1) { dst[i] = '\0'; return 0; }
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return 1;
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

int fs_init() {
    if (name_set("root\0", root_folder.name)) {return 0;}
    target_folder = &root_folder;
    return 1;
}

int fs_return() {
    target_folder = &root_folder;
    return 1;
}

int fs_wipe(folder* f) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        for (int j = 0; j < MAX_NAME_LENGTH; j++) {
            f->files[i].name[j] = 0;
            f->folders[i]->name[j] = 0;
        }
        f->files[i].start_block = 0;
        f->files[i].t_size = 0;
        fs_wipe(f->folders[i]);
    }
    return 1;
}

int fs_mkdir(char* name, folder* f) {
    for (int i=0; i<MAX_ITEMS; i++) {
        if (!str_eq(f->folders[i]->name, "\0")) {
            name_set(name, f->folders[i]->name);
            fs_wipe(f->folders[i]);
            return 1;
        }
    }
    return 0;
}

#endif
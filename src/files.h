#ifndef FILES_H
#define FILES_H

#define MAX_NAME_LENGTH 32
#define MAX_ITEMS 64

#include <stdint.h>


#define UART0_BASE 0x10000000UL
#define UART_RBR (UART0_BASE + 0x00)  // Receiver Buffer Register (read)
#define UART_THR (UART0_BASE + 0x00)  // Transmit Holding Register (write)
#define UART_LSR (UART0_BASE + 0x05)  // Line Status Register

#define UART_LSR_DR   0x01  // Data Ready
#define UART_LSR_THRE 0x20  // Transmitter Holding Register Empty

static void putc(char c) {
    volatile unsigned char *thr = (unsigned char *)UART_THR;
    volatile unsigned char *lsr = (unsigned char *)UART_LSR;

    // Wait for THR empty
    while ((*lsr & UART_LSR_THRE) == 0)
        ;
    *thr = c;
}

static void puts(const char *s) {
    while (*s)
        putc(*s++);
}

static int getc(void) {
    volatile unsigned char *rbr = (unsigned char *)UART_RBR;
    volatile unsigned char *lsr = (unsigned char *)UART_LSR;

    // Wait until Data Ready bit is set
    while ((*lsr & UART_LSR_DR) == 0)
        ;
    return *rbr;
}

typedef struct file file;
typedef struct folder folder;

struct file {
  char name[MAX_NAME_LENGTH];
  char *content;
};

struct folder {
  char name[MAX_NAME_LENGTH];
  file *files[MAX_ITEMS];
  folder *folders[MAX_ITEMS];
};

static folder root_folder;
static folder *target_folder;

int name_set(const char *src, char dst[MAX_NAME_LENGTH]) {
  int i = 0;
  if (!src || !dst)
    return 0;
  while (src[i] != '\0') {
    if (i >= MAX_NAME_LENGTH - 1) {
      dst[i] = '\0';
      return 0;
    }
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
  if (name_set("root\0", root_folder.name)) {
    return 0;
  }
  target_folder = &root_folder;
  return 1;
}

int fs_return() {
  target_folder = &root_folder;
  return 1;
}

int fs_wipe(folder *f) {
  for (int i = 0; i < MAX_ITEMS; i++) {
    for (int j = 0; j < MAX_NAME_LENGTH; j++) {
      f->files[i]->name[j] = 0;
      f->folders[i]->name[j] = 0;
    }
    f->files[i]->content = 0;

    if (!str_eq(f->folders[i]->name, "\0")) {
      fs_wipe(f->folders[i]);
    }
  }
  return 1;
}

int fs_mkdir(char *name, folder *f) {
  static folder folder_pool[MAX_ITEMS];
  static int folder_index = 0;

  for (int i = 0; i < MAX_ITEMS; i++) {
    if (str_eq(f->folders[i]->name, "\0")) {
      if (folder_index >= MAX_ITEMS)
        return 0;
      f->folders[i] = &folder_pool[folder_index++];
      fs_wipe(f->folders[i]);
      name_set(name, f->folders[i]->name);
      puts("\ncreated ");
      puts(name);
      return 1;
    }
  }
  return 0;
}

int fs_ls(folder *f) {
  puts("*folders*\n");
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (!str_eq(f->folders[i]->name, "\0")) {
      puts(f->folders[i]->name);
      puts("\n");
    }
  }
  puts("\n*files*\n");
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (!str_eq(f->files[i]->name, "\0")) {
      puts(f->files[i]->name);
      puts("\n");
    }
  }
  return 0;
}

int fs_mkfile(char *name, folder *f) {
  static file file_pool[MAX_ITEMS];
  static int file_index = 0;

  for (int i = 0; i < MAX_ITEMS; i++) {
    if (str_eq(f->files[i]->name, "\0")) {
      if (file_index >= MAX_ITEMS)
        return 0;
      f->files[i] = &file_pool[file_index++];
      name_set(name, f->files[i]->name);
      puts("\ncreated ");
      puts(name);
      return 1;
    }
  }
  return 0;
}

int fs_cd(folder *f, char *name) {
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (str_eq(f->folders[i]->name, name)) {
      target_folder = f->folders[i];
      return 1;
    }
    if (str_eq(f->folders[i]->name, "\0")) {
      return 0;
    }
  }
  return 0;
}

file *get_file(char *name) {
  file *res;
  for (int i = 0; i < MAX_ITEMS; i++) {
    if (str_eq(target_folder->files[i]->name, name)) {
      res = target_folder->files[i];
      return res;
    }
  }
  return (file *)0;
}

int fs_write(file *f, char *content) {
  if (!f) {
    return 0;
  }
  f->content = content;
  return 1;
}

char *fs_read(const file *f) {
  if (!f)
    return 0;
  return f->content;
}

#endif

TARGET := riscv32-unknown-none
AS := riscv64-elf-as
CC := riscv64-elf-gcc
CCFLAGS := -Wall -ggdb -ffreestanding -nostdlib -I./include -mabi=ilp32 -g -O0
RISCVFLAGS := -march=rv32i 
LD := riscv64-elf-ld
VERSION := debug


SRC_DIR := src
ASM_DIR := $(SRC_DIR)/asm
BOOT_ASM := $(ASM_DIR)/start.S
LINKER_SCRIPT := linker.ld

KERNEL_C := $(SRC_DIR)/kernel.c

BOOT_OBJ := $(ASM_DIR)/boot.o
KERNEL_OBJ_DIR := target/$(TARGET)/$(VERSION)
KERNEL_OBJ := $(KERNEL_OBJ_DIR)/lib.a
KERNEL_ELF := kernel.elf

QEMU := qemu-system-riscv32
QEMU_FLAGS := -M virt -bios none -serial stdio -kernel ${KERNEL_ELF}

all: $(KERNEL_ELF)

$(BOOT_OBJ): $(BOOT_ASM)
	$(AS) $(RISCVFLAGS) -c $< -o $@

$(KERNEL_OBJ): $(KERNEL_C)
	$(CC) $(RISCVFLAGS) $(CCFLAGS) -c $< -o $@

$(KERNEL_ELF): $(BOOT_OBJ) $(KERNEL_OBJ) $(LINKER_SCRIPT)
	$(LD) -o $(KERNEL_ELF) -T $(LINKER_SCRIPT) -m elf32lriscv -o $@ $(BOOT_OBJ) $(KERNEL_OBJ)
 
clean:
	rm -rf target
	rm -f $(BOOT_OBJ) $(KERNEL_ELF)
	mkdir -p $(KERNEL_OBJ_DIR)

run:
	$(QEMU) $(QEMU_FLAGS)

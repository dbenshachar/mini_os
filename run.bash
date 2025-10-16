#!/bin/bash

# Use aarch64 (ARM64) target instead of x86
aarch64-elf-gcc -ffreestanding -nostdlib -Wl,--build-id=none -Wl,-Ttext=0x40000000 -o kernel.bin kernel.c

# Run in QEMU for ARM64
qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel kernel.bin
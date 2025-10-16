#!/bin/bash
set -e

nasm -f bin boot.asm -o boot.bin
dd if=/dev/zero of=boot.img bs=512 count=2880 2>/dev/null
dd if=boot.bin of=boot.img conv=notrunc 2>/dev/null
qemu-system-i386 -drive format=raw,file=boot.img
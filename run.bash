osascript <<APPLESCRIPT
tell application "Terminal"
  do script "cd '$(pwd)' && bash -lc 'set -euo pipefail; \
  aarch64-elf-as start.S -o start.o; \
  aarch64-elf-gcc -ffreestanding -fno-pic -c kernel.c -o kernel.o; \
  aarch64-elf-ld -nostdlib -T linker.ld -o kernel.elf start.o kernel.o; \
  qemu-system-aarch64 -M virt -cpu cortex-a57 -nographic -serial mon:stdio -kernel kernel.elf; \
  exec bash -i'"
  activate
end tell
APPLESCRIPT

; boot.asm — 512-byte boot sector (BIOS, real mode)
bits 16
org 0x7C00
    mov si, msg
.print:
    lodsb
    or   al, al
    jz   .hang
    mov  ah, 0x0E
    int  0x10
    jmp  .print
.hang: jmp .hang
msg db "Hello from boot sector!",0

times 510-($-$$) db 0
dw 0xAA55
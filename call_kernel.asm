bits 32

global _start

_start:
    mov ax, 0xB800
    mov es, ax

    mov word [es:0], 0x4F58

hang:
    jmp hang
bits 32

global _start
extern _kernel

_start:
    call _kernel

hang:
    jmp hang
; boot.asm
; this is the bot loader is a mini bootloader
bits 16
org 0x7C00

start:

    cli

    mov [boot_drive], dl

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov sp, 0x7C00

    mov ah, 0x02
    mov al, 25
    mov ch, 0
    mov cl, 2
    mov dh, 0

    mov dx, [boot_drive]

    mov bx, 0x1000
    mov es, bx
    xor bx, bx

    int 0x13
    jc disk_error

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

disk_error:
    mov si, error_msg

print_error:
    lodsb
    test al, al
    jz halt

    mov ah, 0x0E
    int 0x10

    jmp print_error

halt:
    cli
    hlt
    jmp halt

error_msg db "KERNEL LOAD ERROR!", 0
boot_drive db 0

gdt_start:
    dq 0

    ; =======================code===============================
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0 << 2) | (1 << 1) | 0
    db (1 << 7) | (1 << 6) | 0x0F
    db 0x00

    ;=======================data==================================
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db (1 << 1) | (1 << 4) | (1 << 7)
    db (1 << 7) | (1 << 6) | 0x0F
    db 0x00

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start
bits 32

protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    mov esp, 0x90000

    jmp 0x08:0x10000

hang:
    jmp hang


times 510-($-$$) db 0
dw 0xAA55
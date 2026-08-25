#!/bin/bash

set -e

echo "[1/6] Building bootloader..."
nasm -f bin boot.asm -o boot.bin

echo "[2/6] Building kernel..."
gcc -m32 -ffreestanding -c kernel.c -o kernel.o

echo "[3/6] Building kernel entry..."
nasm -f elf32 call_kernel.asm -o call_kernel.o

echo "[4/6] Linking..."
ld -m elf_i386 -Ttext 0x10000 \
   -e _start \
   -o call_kernel.elf call_kernel.o kernel.o

echo "[5/6] Converting to binary..."
objcopy -O binary call_kernel.elf call_kernel.bin

echo "[6/6] Padding to 40 sectors..."
python3 - <<'PY'
p = "call_kernel.bin"
target = 40 * 512

with open(p, "rb") as f:
    data = f.read()

if len(data) > target:
    raise SystemExit("ERROR: kernel is larger than 40 sectors!")

with open(p, "wb") as f:
    f.write(data)
    f.write(b"\x00" * (target - len(data)))
PY

cat boot.bin call_kernel.bin > koyot.img

echo "Build successful!"
echo "Kernel size: $(stat -c%s call_kernel.bin) bytes"
echo "Image size:  $(stat -c%s koyot.img) bytes"

run() {
    qemu-system-x86_64 -drive format=raw,file=koyot.img
}
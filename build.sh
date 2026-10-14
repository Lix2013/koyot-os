#!/bin/bash

# Copyright 2026 Lix
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


set -e

# إنشاء مجلد البناء إذا لم يكن موجوداً
mkdir -p target

build() {
    echo "[1/6] Building bootloader..."
    nasm -f bin boot.asm -o target/boot.bin

    echo "[2/6] Building kernel..."
    # إضافة خيارات إيقاف الحماية والحاويات لضمان كود صافي للـ Bare-Metal
    gcc -m32 -ffreestanding -fno-stack-protector -fno-pie -c kernel.c -o target/kernel.o

    echo "[3/6] Building kernel entry..."
    nasm -f elf32 call_kernel.asm -o target/call_kernel.o

    echo "[4/6] Linking..."
    ld -m elf_i386 -Ttext 0x10000 \
        -e _start \
        -o target/call_kernel.elf target/call_kernel.o target/kernel.o

    echo "[5/6] Converting to binary..."
    objcopy -O binary target/call_kernel.elf target/call_kernel.bin

    echo "[6/6] Padding to 40 sectors..."
    python3 - <<'PY'
import os

p = "target/call_kernel.bin"
target = 40 * 512

with open(p, "rb") as f:
    data = f.read()

if len(data) > target:
    raise SystemExit(f"ERROR: kernel ({len(data)} bytes) is larger than 40 sectors ({target} bytes)!")

with open(p, "wb") as f:
    f.write(data)
    f.write(b"\x00" * (target - len(data)))
PY

    # تصحيح مسارات الدمج من داخل مجلد target
    cat target/boot.bin target/call_kernel.bin > target/koyot-v0.2.img

    echo "======================================"
    echo "Build successful!"
    echo "Kernel size: $(stat -c%s target/call_kernel.bin) bytes"
    echo "Image size:  $(stat -c%s target/koyot-v0.2.img) bytes"
    echo "======================================"
}

run() {
    if [ ! -f target/koyot-v0.2.img ]; then
        echo "Image not found! Building first..."
        build
    fi
    qemu-system-i386 -drive format=raw,file=target/koyot-v0.2.img
}

echo "Koyot v0.2 build system loaded."
echo "Commands: build | run"
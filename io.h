/*
 * Copyright 2026 Lix
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IO_H
#define IO_H

#include "keyboard.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static volatile unsigned short* const VGA_MEMORY = (volatile unsigned short*)0xB8000;

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned char text_color = 0x0F;

static inline void outb(unsigned short port, unsigned char val)
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline int strcmp(const char* a, const char* b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

static inline void update_hardware_cursor(void)
{
    unsigned short pos = cursor_y * VGA_WIDTH + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

static inline void set_color(unsigned char fg, unsigned char bg)
{
    text_color = (bg << 4) | (fg & 0x0F);
}

static inline void putchar(char c)
{
    if (c == '\n')
    {
        cursor_x = 0;
        cursor_y++;
    }
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    else if (c == '\b')
    {
        if (cursor_x > 0)
        {
            cursor_x--;
            VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = ((unsigned short)text_color << 8) | ' ';
        }
    }
    else
    {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = ((unsigned short)text_color << 8) | (unsigned char)c;
        cursor_x++;

        if (cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT)
    {
        for (int y = 1; y < VGA_HEIGHT; y++)
        {
            for (int x = 0; x < VGA_WIDTH; x++)
            {
                VGA_MEMORY[(y - 1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
            }
        }

        for (int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((unsigned short)text_color << 8) | ' ';
        }

        cursor_y = VGA_HEIGHT - 1;
    }

    update_hardware_cursor();
}

static inline void print(const char* text)
{
    while (*text)
    {
        putchar(*text);
        text++;
    }
}

static inline void clear(void)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        VGA_MEMORY[i] = ((unsigned short)text_color << 8) | ' ';
    }

    cursor_x = 0;
    cursor_y = 0;
    update_hardware_cursor();
}

static inline void input(const char* prompt, char* buffer, int max_length)
{
    print(prompt);

    int i = 0;

    while (i < max_length - 1)
    {
        unsigned char key = get_key();

        if (key == '\n')
        {
            putchar('\n');
            break;
        }

        if (key == '\b')
        {
            if (i > 0)
            {
                i--;
                putchar('\b');
            }
            continue;
        }

        if (key != 0)
        {
            buffer[i] = key;
            i++;
            putchar(key);
        }
    }

    buffer[i] = '\0';
}

#endif
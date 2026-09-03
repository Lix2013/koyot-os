#ifndef IO_LIB_H
#define IO_LIB_H

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#include "keyboard.h"

int strcmp(const char* a, const char* b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }

    return *a - *b;
}

volatile unsigned short* const VGA_MEMORY =
    (volatile unsigned short*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;

unsigned char text_color = 0x0F;


void putchar(char c)
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
            VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] =
                ((unsigned short)text_color << 8) | ' ';
        }
    }
    else
    {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] =
            ((unsigned short)text_color << 8) | (unsigned char)c;

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
                VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                    VGA_MEMORY[y * VGA_WIDTH + x];
            }
        }

        for (int x = 0; x < VGA_WIDTH; x++)
        {
            VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
                ((unsigned short)text_color << 8) | ' ';
        }

        cursor_y = VGA_HEIGHT - 1;
    }
}

void print(const char* text)
{
    while (*text)
    {
        putchar(*text);
        text++;
    }
}


void input(const char* prompt, char* buffer, int max_length)
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

        if (key == 0) {
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

void clear(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    for (int i = 0; i < 80 * 25; i++)
    {
        video[i] = 0x0F20;
    }

    cursor_x = 0;
    cursor_y = 0;
}

#endif
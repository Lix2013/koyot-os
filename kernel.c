#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile unsigned short* const VGA_MEMORY =
    (volatile unsigned short*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;

unsigned char text_color = 0x0F;

#define VERSION "0.2"

const char keyboard_map[128] = {
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = '\b',
    [0x0F] = '\t',

    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = '\n',

    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',

    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    
    [0x39] = ' '
};

int strcmp(const char* a, const char* b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }

    return *a - *b;
}

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


unsigned char inb(unsigned short port)
{
    unsigned char value;

    asm volatile (
        "inb %1, %0"
        : "=a"(value)
        : "Nd"(port)
    );

    return value;
}

void clear(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    for (int i = 0; i < 80 * 25; i++)
    {
        video[i] = 0x0F20;
    }
}

unsigned char get_scancode()
{
    while ((inb(0x64) & 1) == 0)
        ;

    return inb(0x60);
}

unsigned char get_key() {
    char key = get_scancode();

    return keyboard_map[key];
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

void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    clear();


    print("Hello from Koyot kernel !\n");

    char buffer[128];

    while (1) {
        input("enter command > ", buffer, 128);

        if (strcmp(buffer, "q") == 0) {
            break;
        } else if (strcmp(buffer, "clear") == 0) {
            clear();
        } else if (strcmp(buffer, "ver") == 0) {
            print("Koyot OS version : <");
            print(VERSION);
            print(">\n");
        }
    }

    print("Bye !\n");

    while (1)
    {
        
        __asm__ volatile ("hlt");
    }
}
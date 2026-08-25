#define VGA_WIDTH 80
#define VGA_HEIGHT 25

volatile unsigned short* const VGA_MEMORY =
    (volatile unsigned short*)0xB8000;

int cursor_x = 0;
int cursor_y = 0;

unsigned char text_color = 0x0F;

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

    if (key == 0x10) {
        return 'q';
    }
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
        // Scroll الشاشة سطرًا واحدًا للأعلى
        for (int y = 1; y < VGA_HEIGHT; y++)
        {
            for (int x = 0; x < VGA_WIDTH; x++)
            {
                VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                    VGA_MEMORY[y * VGA_WIDTH + x];
            }
        }

        // مسح آخر سطر
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

void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    clear();


    print("Hello from Koyot kernel !\n");

    while (1)
    {
        char key = get_key();
        
        __asm__ volatile ("hlt");
    }
}
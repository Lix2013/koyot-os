void clear(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    for (int i = 0; i < 80 * 25; i++)
    {
        video[i] = 0x0F20;
    }
}

void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    clear();

    const char* msg = "Hello from Koyot kernel!";

    for (int i = 0; msg[i] != '\0'; i++)
    {
        video[i] = (unsigned short)msg[i] | (0x0F << 8);
    }

    while (1)
    {
        __asm__ volatile ("hlt");
    }
}
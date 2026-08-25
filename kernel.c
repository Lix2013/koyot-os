void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

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
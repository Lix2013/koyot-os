//kernel.cpp
//this is the kernel of system

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

int streq(const char* a, const char* b)
{
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return 0;

        i++;
    }

    return a[i] == b[i];
}


void clear(volatile unsigned short* video) { 
    for (int i = 0; i < 80 * 25; i++) { 
        video[i] = 0x0F20; 
    } 
} 


void print(const char* text, volatile unsigned short* video, int position,const char *str_color="WHITE") { 
    int color = 0x0F;
    if (streq(str_color, "BLUE")) {
        color = 0x01;
    }
    if (streq(str_color, "GREEN")) {
        color = 0x02;
    }
    if (streq(str_color, "CYAN")) {
        color = 0x03;
    }
    if (streq(str_color, "WHITE")) {
        color = 0x0F;
    }

    for (int i = 0; text[i] != '\0'; i++)
    {
        video[position + i] = (color << 8) | text[i];
    }
}


extern "C" void kernel(void) {
    volatile unsigned short* video = (volatile unsigned short*)0xB8000;

    clear(video);
    print("Hello !", video, 0, "CYAN");
    print("Koyot OS - 0.1 - LTS", video, 80, "BLUE");
    print("You are in console !", video, 160, "GREEN");

    while (1){
        unsigned char key = inb(0x60);
        if (key == 0x10) {
            print("q pressed !", video, 240);
        }
    }
}
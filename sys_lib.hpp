#ifndef SYS_LIB_HPP
#define SYS_LIB_HPP

//for strings
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

//print strings
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


//clear console
void clear(volatile unsigned short* video) { 
    for (int i = 0; i < 80 * 25; i++) { 
        video[i] = 0x0F20; 
    } 
} 




#endif
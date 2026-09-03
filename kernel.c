#include "io.h"
#include "keyboard.h"

#define VERSION "0.3"
void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    clear();

    print("----Koyot OS V0.8 LTS - you are in kernel----\n");

    char buffer[128];

    while (1) {
        input("> ", buffer, 128);

        if (strcmp(buffer, "q") == 0) {
            break;
        } else if (strcmp(buffer, "clear") == 0) {
            clear();
            print("----Koyot OS V0.8 LTS - you are in kernel----\n");
        } else if (strcmp(buffer, "ver") == 0) {
            print("Koyot OS version : <");
            print(VERSION);
            print(">\n");
        }
    }

    clear();
    print("----Koyot OS V0.8 LTS - you are in kernel----\n");
    print("Bye !\n");
}
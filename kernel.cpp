//kernel.cpp
//this is the kernel of system
#include "sys_lib.hpp"
#include "keyboard.hpp"

//kernel
extern "C" void kernel(void) {
    volatile unsigned short* video = (volatile unsigned short*)0xB8000;

    clear(video);
    print("KERNEL NEW!", video, 0, "RED");
    print("Koyot OS - 0.1 - LTS", video, 80, "BLUE");
    print("You are in console !", video, 160, "GREEN");

    char buffer[128];

    input(buffer, 128);

    print(buffer, video, 240);
}
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

#include "io.h"
#include "keyboard.h"

#define VERSION "0.9 LTS"

static inline void print_logo(void)
{
    set_color(0x0B, 0x00);

    print("\n");
    print("  _  _____      _____ _____   ____   _____ \n");
    print(" | |/ / _ \\ \\ / / _ |_   _| / __ \\ / ____|\n");
    print(" | ' / | | \\ V / | | || |  | |  | | (___  \n");
    print(" |  <| | | || || | | || |  | |  | |\\___ \\ \n");
    print(" | . \\ |_| || || |_| || |  | |__| |____) |\n");
    print(" |_|\\_\\___/ |_| \\___/ |_|   \\____/|_____/ \n");
    print("\n");

    // إعادة اللون للأبيض الافتراضي
    set_color(0x0F, 0x00);
    print("      Koyot OS v0.9 LTS - The Kernel\n");
    print("      ------------------------------\n\n");
}

/// @brief 
/// @param  
void _kernel(void)
{
    volatile unsigned short* video =
        (volatile unsigned short*)0xB8000;

    clear();

    print_logo();

    char buffer[128];

    while (1) {
        input("> ", buffer, 128);

        if (strcmp(buffer, "q") == 0) {
            break;
        } else if (strcmp(buffer, "clear") == 0) {
            clear();
            print_logo();
        } else if (strcmp(buffer, "ver") == 0) {
            set_color(0x3, 0x01);
            print("Koyot OS version : <");
            print(VERSION);
            print(">\n");
            set_color(0x0F, 0x00);
        } else if (strcmp(buffer, "koyot-os") == 0) {
            print_logo();
        } else {
            print("Unknown command: ");
            print(buffer);
            print("\n");
        }
    }

    clear();
    for (int i = 0; i < 100; i++) {
        print("Shutting down \n");
    }
    clear();
    print_logo();
    print("Bye !\n");
}
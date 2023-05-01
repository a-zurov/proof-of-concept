// https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux
//--------------------------------------------------------------------------------------------

#pragma once

#ifndef _WIN32

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <glob.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>


static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* make new settings same as old settings */
    current.c_lflag &= ~ICANON; /* disable buffered i/o */
    if (echo) {
        current.c_lflag |= ECHO; /* set echo mode */
    }
    else {
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char _getch(void)
{
    return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
    return getch_(1);
}

/*
// Return -1 if no key is being pressed, or else the lowest keycode (c.f. linux/input-event-codes.h)
// of all the keys that are being pressed

int keycode_of_key_being_pressed() {
    FILE* kbd;
    glob_t kbddev; // Glob structure for keyboard devices
    glob("/dev/input/by-path/*-kbd", 0, 0, &kbddev); // Glob select all keyboards
    int keycode = -1; // keycode of key being pressed
    for (int i = 0; i < kbddev.gl_pathc; i++) { // Loop through all the keyboard devices ...
        if (!(kbd = fopen(kbddev.gl_pathv[i], "r"))) { // ... and open them in turn (slow!)
            perror("Run as root to read keyboard devices");
            exit(1);
        }

        char key_map[KEY_MAX / 8 + 1]; // Create a bit array the size of the number of keys
        memset(key_map, 0, sizeof(key_map)); // Fill keymap[] with zero's
        ioctl(fileno(kbd), EVIOCGKEY(sizeof(key_map)), key_map); // Read keyboard state into keymap[]
        for (int k = 0; k < KEY_MAX / 8 + 1 && keycode < 0; k++) { // scan bytes in key_map[] from left to right
            for (int j = 0; j < 8; j++) { // scan each byte from lsb to msb
                if (key_map[k] & (1 << j)) { // if this bit is set: key was being pressed
                    keycode = 8 * k + j; // calculate corresponding keycode
                    printf("keycode %d", keycode);
                    break; // don't scan for any other keys
                }
            }
        }

        fclose(kbd);
        if (keycode)
            break; // don't scan for any other keyboards
    }
    return keycode;
}
*/

#endif // NOT _WIN32

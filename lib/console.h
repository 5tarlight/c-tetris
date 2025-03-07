#ifndef TETRIS_CONSOLE_H
#define TETRIS_CONSOLE_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

#define KEY_UP 1000
#define KEY_DOWN 1001
#define KEY_LEFT 1002
#define KEY_RIGHT 1003

void clear_console();
int read_key();
void flush_input();

#endif //TETRIS_CONSOLE_H

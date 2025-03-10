#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"
#include "util.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define PADDING 8

#define BLOCK_FILLED "■"
#define BLOCK_EMPTY "□"
#define BLOCK_FALLING "▣"

#define BORDER_HORIZONTAL "─"
#define BORDER_VERTICAL "│"
#define BORDER_TOP_LEFT "┌"
#define BORDER_TOP_RIGHT "┐"
#define BORDER_BOTTOM_LEFT "└"
#define BORDER_BOTTOM_RIGHT "┘"

void init_game();
void end_game();
void draw_title();
void draw_board();

Pair get_block_position(int x, int y);
void fill_block(int x, int y, const char* block, const char* color);

#endif // TETRIS_GAME_H

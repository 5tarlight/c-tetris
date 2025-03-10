#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"
#include "util.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 15
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

#define BLOCK_COUNT 7

void init_game();
void end_game();
void draw_title();
void draw_board();

Pair get_block_position(int x, int y);
void fill_block(int x, int y, const char* block, const char* color);
int* generate_7bag();
int get_next_block();

void spawn_block();
void move_block(int dx, int dy);
void rotate_block_left();
void rotate_block_right();
void drop_block();
int can_move_block(int dx, int dy);
int can_rotate_block(int dr);
void fix_block();
void clear_lines();
void draw_block();
void draw_next_block();

#endif // TETRIS_GAME_H

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

/**
 * 블록의 위치를 실제 콘솔의 좌표로 변환하는 함수
 *
 * @param x 블록의 x 좌표
 * @param y 블록의 y 좌표
 * @return Pair 콘솔의 좌표
 */
Pair get_block_position(int x, int y);

/**
 * 블록을 콘솔에 그리는 함수.
 * 입력받은 좌표를 `get_block_position` 함수를 통해 변환하고,
 * 해당 좌표에 블록을 그린다.
 *
 * @param x 블록의 x 좌표
 * @param y 블록의 y 좌표
 * @param block 블록 종류
 * @param color 블록 색상
 * @see get_block_position
 */
void fill_block(int x, int y, const char* block, const char* color);

/**
 * 7-bag을 생성해서 반환하는 함수.
 *
 * @return int* 7-bag을 담는 배열
 */
int* generate_7bag();

/**
 * 다음 블록을 가져옴. 7-bag이 비어있으면 새로 섞어서 가져옴.
 */
int get_next_block();

/**
 * 새 블록을 콘솔에 그린다.
 */
int spawn_block();

void move_block(int dx, int dy);
void rotate_block_left();
void rotate_block_right();

/**
 * 블록을 최대한 아래로 내린다.
 */
void drop_block();

int can_move_block(int dx, int dy);
int can_rotate_left();
int can_rotate_right();
int can_spawn_block();

/**
 * 떨어지던 블록이 바닥에 닿았을 때 board를 업데이트한다.
 */
void fix_block();

/**
 * 가로줄이 전부 채워졌는지 확인하고 채워진 줄을 지운다.
 * 줄을 지우면서 점수를 계산한다.
 */
void clear_lines();

/**
 * 현재 블록을 콘솔에 그린다.
 *
 * 주의 : 만약 기존에 블록이 있었다면 지워야 한다.
 */
int draw_block();

/**
 * 다음 스폰될 블록을 왼쪽 메뉴에 그린다.
 */
void draw_next_block();

/**
 * 현재 출력된 블록을 지운다. 블록이 움직이기 전 실행해서
 * 이전 블록을 지우고 새로운 블록을 그릴 수 있도록 한다.
 */
void erase_block();

/**
 * 블록의 라이프사이클을 제어하는 함수.
 */
int control_block();

/**
 * 게임 보드를 콘솔에 출력한다.
 * 디버그용.
 */
void print_board();

#endif // TETRIS_GAME_H

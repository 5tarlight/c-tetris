#include "game.h"
#include "console.h"
#include <stdio.h>

void init_game() {
  srand(time(NULL));
  hide_cursor();
  draw_title();
  draw_board();
}

void end_game() {
  move_cursor(0, BOARD_HEIGHT + 1);
  show_cursor();
}

void draw_title() {
  move_cursor(0, 0); 
  printf("%sT", FG_RED);
  printf("%sE", FG_GREEN);
  printf("%sT", FG_YELLOW);
  printf("%sR", FG_BLUE);
  printf("%sI", FG_MAGENTA);
  printf("%sS", FG_CYAN);
  printf("%s", RESET);  
}

#define LOOP(n) for (int i = 0; i < (n); i++)

void draw_board() {
  LOOP(BOARD_HEIGHT) {
    move_cursor(PADDING, i);
    printf("%s", BORDER_VERTICAL);
    LOOP(BOARD_WIDTH)
      printf("%s", BLOCK_EMPTY);
    printf("%s\n", BORDER_VERTICAL);
  }

  move_cursor(PADDING, BOARD_HEIGHT);
  printf("%s", BORDER_BOTTOM_LEFT);
  LOOP(BOARD_WIDTH)
    printf("%s", BORDER_HORIZONTAL);
  printf("%s\n", BORDER_BOTTOM_RIGHT);
}

Pair get_block_position(int x, int y) {
  return (Pair){ PADDING + x, y };
}

void fill_block(int x, int y, const char* block, const char* color) {
  Pair pos = get_block_position(x, y);
  move_cursor(pos.x, pos.y);
  printf("%s%s%s\n", color, block, RESET);
}

int block[7][4][4][4] = {
  { // T
      {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,0,0 }, { 0,1,1,0 }, { 0,1,0,0 }},
      {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,0 }, { 0,1,0,0 }},
      {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }}
  },
  { // Z
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }},
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }}
  },
  { // S
      {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }},
      {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }}
  },
  { // I
      {{ 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }},
      {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,1 }, { 0,0,0,0 }},
      {{ 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }},
      {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,1 }, { 0,0,0,0 }}
  },
  { // L
      {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }, { 1,0,0,0 }},
      {{ 0,0,0,0 }, { 1,1,1,0 }, { 0,0,1,0 }, { 0,0,0,0 }},
      {{ 0,1,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 0,0,0,0 }}
  },
  { // J
      {{ 0,0,0,0 }, { 0,0,1,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
      {{ 1,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 1,1,1,0 }, { 1,0,0,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }}
  },
  { // O
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
      {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }}
  }
};

#include "game.h"
#include "console.h"
#include <stdio.h>

void init_game() {
  srand(time(NULL));
  draw_title();
  draw_board();
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
#include "game.h"
#include "console.h"
#include <stdio.h>

void init_game() {
  srand(time(NULL));
  draw_title();
  draw_board();
}

void draw_title() {
  move_cursor(PADDING, PADDING); 
  printf("%sT ", FG_RED);
  printf("%sE ", FG_GREEN);
  printf("%sT ", FG_YELLOW);
  printf("%sR ", FG_BLUE);
  printf("%sI ", FG_MAGENTA);
  printf("%sS", FG_CYAN);
}

void draw_board() {}
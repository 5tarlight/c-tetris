#include <stdio.h>
#include "lib/console.h"

int main() {
  Pair size = get_console_size();
  clear_console();
  printf("Console size: %d x %d\n\n", size.x, size.y);

  set_console_color(FG_MAGENTA);
  set_console_color(BG_YELLOW);
  for (int i = 5; i < 10; i++) {
    for (int j = 5; j < 10; j++) {
      move_cursor(j, i);
      printf("*");
      SLEEP(10);
    }
  }
  reset_console_color();
  printf("\n");

  return 0;
}

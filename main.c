#include "lib/console.h"

int main() {
  Pair size = get_console_size();
  clear_console();
  printf("Console size: %d x %d\n\n", size.x, size.y);

  int x = 3;
  int y = 3;
  while (1) {
    int key = read_key();
    if (key == 'q') break;

    move_cursor(x, y);
    printf("#");
    x++; y++;

    SLEEP(100);
  }
  
  return 0;
}

#include <stdio.h>
#include "lib/console.h"
#include "lib/game.h"

int main() {
  init_console();
  clear_console();
  init_game();

  // while (1) {
  //   int key = read_key();
  //   if (key)
  //     printf("%d\n", key);
  //   SLEEP(100);
  // }

  while (1) {
    spawn_block();
    draw_next_block();
    int code = control_block(); // This causes lock!
    clear_lines();

    if (code == -1) break;
  }

  print_board();
//  end_game();

  return 0;
}

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
    control_block(); // This causes lock!
    clear_lines();
//    break;
  }

  end_game();
//  print_board();

  return 0;
}

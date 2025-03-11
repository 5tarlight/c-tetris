#include <stdio.h>
#include "lib/console.h"
#include "lib/game.h"

int main() {
  init_console();
  clear_console();
  init_game();

  while (1) {
    int code = spawn_block();
    draw_next_block();

    if (code == -1) break;

    code = control_block(); // This causes lock!
    clear_lines();

    if (code == -1) break;
  }

//  print_board();
  end_game();

  return 0;
}

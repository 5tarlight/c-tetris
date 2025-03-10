#include <stdio.h>
#include "lib/console.h"
#include "lib/game.h"

int main() {
  init_console();
  clear_console();
  init_game();

  while (1) {
    spawn_block();

    SLEEP(200);
  }

  end_game();

  return 0;
}

#include <stdio.h>
#include "lib/console.h"
#include "lib/game.h"

int main() {
  init_console();
  clear_console();
  init_game();

  SLEEP(1000);
  
  fill_block(5, 5, BLOCK_FILLED, FG_RED);

  end_game();

  return 0;
}

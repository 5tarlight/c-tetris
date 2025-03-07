#include "lib/console.h"

int main() {
  while (1) {
    clear_console();
    int key = read_key();

    if (key) {
      printf("Key = %d\n", key);
    } else {
      printf("Waiting for input\n");
    }

    SLEEP(100);
    
    if (key == 'q') {
      break;
    }
  }
  
  return 0;
}

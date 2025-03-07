#include "console.h"

void clear_console() {
#ifdef _WIN32
  system("cls");
#elif defined(__APPLE__) || defined(__linux__)
  system("clear");
#endif
}

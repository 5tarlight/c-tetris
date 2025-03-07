#include "console.h"

void clear_console() {
#ifdef _WIN32
  system("cls");
#elif defined(__APPLE__) || defined(__linux__)
  system("clear");
#endif
}

int _read_key() {
#ifdef _WIN32
  if (_kbhit()) {
    int ch = _getch();
    if (ch == 224) { // 방향키는 224 + 코드 조합
      ch = _getch();
      switch (ch) {
      case 72:
        return KEY_UP;
      case 80:
        return KEY_DOWN;
      case 75:
        return KEY_LEFT;
      case 77:
        return KEY_RIGHT;
      }
    }
    return ch;
  }
  return 0;
#else
  struct termios oldt, newt;
  int ch;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  if (ch == 27) {
    if (getchar() == '[') {
      switch (getchar()) {
      case 'A':
        return KEY_UP;
      case 'B':
        return KEY_DOWN;
      case 'C':
        return KEY_RIGHT;
      case 'D':
        return KEY_LEFT;
      }
    }
  }

  return ch == EOF ? 0 : ch;
#endif
}

int read_key() {
  int ch = _read_key();
  flush_input();
  return ch;
}

void flush_input() {
#ifdef _WIN32
  while (_kbhit())
    _getch(); // Windows는 남은 입력 싹 비우기
#else
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_cc[VTIME] = 0;
  newt.c_cc[VMIN] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  while (getchar() != EOF); // 입력이 없을 때까지 계속 읽기

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

#define KEY_UP 1000
#define KEY_DOWN 1001
#define KEY_LEFT 1002
#define KEY_RIGHT 1003

#define RESET "\033[0m"
#define FG_BLACK "\033[30m"
#define FG_RED "\033[31m"
#define FG_GREEN "\033[32m"
#define FG_YELLOW "\033[33m"
#define FG_BLUE "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN "\033[36m"
#define FG_WHITE "\033[37m"
#define BG_CYAN "\033[46m"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 15
#define PADDING 8

#define BLOCK_FILLED "■"
#define BLOCK_EMPTY "□"

#define BORDER_HORIZONTAL "─"
#define BORDER_VERTICAL "│"
#define BORDER_BOTTOM_LEFT "└"
#define BORDER_BOTTOM_RIGHT "┘"

typedef struct {
  int x;
  int y;
} Pair;

#ifndef _WIN32
int min(int, int);
int max(int, int);
#endif

void clear_console();
int read_key();
void flush_input();
void move_cursor(int x, int y);
void init_console();
void hide_cursor();
void show_cursor();

void init_game();
void end_game();
void draw_title();
void draw_board();

Pair get_block_position(int x, int y);
void fill_block(int x, int y, const char* block, const char* color);
int* generate_7bag();
int get_next_block();
int spawn_block();
void move_block(int dx, int dy);
void rotate_block_left();
void rotate_block_right();
void drop_block();
int can_move_block(int dx, int dy);
int can_rotate_left();
int can_rotate_right();
void fix_block();
void clear_lines();
int draw_block();
void draw_next_block();
void erase_block();
int control_block();

#ifndef _WIN32
int min(int a, int b) {
  return a < b ? a : b;
}

int max(int a, int b) {
  return a > b ? a : b;
}
#endif

void clear_console() {
#ifdef _WIN32
  system("cls");
#elif defined(__APPLE__) || defined(__linux__)
  system("clear");
#endif
}

int read_raw_key() {
#ifdef _WIN32
  if (_kbhit()) {
    int ch = _getch();

    if (ch == 224) {
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

  int key = 0;

  if (ch == 27) {
    if (getchar() == '[') {
      switch (getchar()) {
        case 'A': key = KEY_UP; break;
        case 'B': key = KEY_DOWN; break;
        case 'C': key = KEY_RIGHT; break;
        case 'D': key = KEY_LEFT; break;
      }
    }
  } else {
    key = ch == EOF ? 0 : ch;
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return key;
#endif
}

int read_key() {
  int ch = read_raw_key();
  flush_input();
  return ch;
}

void flush_input() {
#ifdef _WIN32
  while (_kbhit())
    _getch();
#else
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_cc[VTIME] = 0;
  newt.c_cc[VMIN] = 0;
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  while (getchar() != EOF);

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
}

Pair get_console_size() {
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

  int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

  return (Pair){ width, height };
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return (Pair){ w.ws_col, w.ws_row };
#endif
}

void move_cursor(int x, int y) {
#ifdef _WIN32
  COORD pos = { x, y };
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
  printf("\033[%d;%dH", y + 1, x + 1);
  fflush(stdout);
#endif
}

void init_console() {
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
  struct termios tty;
  tcgetattr(STDIN_FILENO, &tty);
  tty.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

void hide_cursor() {
  printf("\e[?25l");
  fflush(stdout);
}

void show_cursor() {
  printf("\e[?25h");
  fflush(stdout);
}

void init_game() {
  srand(time(NULL));
  hide_cursor();
  draw_title();
  draw_board();
}

int score = 0;

void end_game() {
  clear_console();

  move_cursor(5, 2);
  printf("Game Over!");
  move_cursor(5, 3);
  printf("Score: %d\n", score);

  show_cursor();
}

void draw_title() {
  move_cursor(0, 0);
  printf("%sT", FG_RED);
  printf("%sE", FG_GREEN);
  printf("%sT", FG_YELLOW);
  printf("%sR", FG_BLUE);
  printf("%sI", FG_MAGENTA);
  printf("%sS", FG_CYAN);
  printf("%s", RESET);
}

#define LOOP(n) for (int i = 0; i < (n); i++)

void draw_board() {
  // 테두리
  LOOP(BOARD_HEIGHT) {
    move_cursor(PADDING, i);
    printf("%s", BORDER_VERTICAL);
    LOOP(BOARD_WIDTH)
      printf("%s", BLOCK_EMPTY);
    printf("%s\n", BORDER_VERTICAL);
  }

  move_cursor(PADDING, BOARD_HEIGHT);
  printf("%s", BORDER_BOTTOM_LEFT);
  LOOP(BOARD_WIDTH)
    printf("%s", BORDER_HORIZONTAL);
  printf("%s\n", BORDER_BOTTOM_RIGHT);

  move_cursor(0, 5);
  printf("%s NEXT %s", BG_CYAN FG_BLACK, RESET);

  move_cursor(22, 0);
  printf("Move   : ←, →, ↓");
  move_cursor(22, 1);
  printf("Rotate : Z, X");
  move_cursor(22, 2);
  printf("Drop   : Space");
}

Pair get_block_position(int x, int y) {
  return (Pair){ PADDING + x, y };
}

void fill_block(int x, int y, const char* block, const char* color) {
  Pair pos = get_block_position(x, y);
  move_cursor(pos.x, pos.y);
  printf("%s%s%s\n", color, block, RESET);
}

int block[7][4][4][4] = {
        { // T
                {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,0,0 }, { 0,1,1,0 }, { 0,1,0,0 }},
                {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,0 }, { 0,1,0,0 }},
                {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }}
        },
        { // Z
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }},
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }}
        },
        { // S
                {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }},
                {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }}
        },
        { // I
                {{ 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }},
                {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,1 }, { 0,0,0,0 }},
                {{ 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }},
                {{ 0,0,0,0 }, { 0,0,0,0 }, { 1,1,1,1 }, { 0,0,0,0 }}
        },
        { // L
                {{ 0,0,0,0 }, { 1,0,0,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 1,1,0,0 }, { 1,0,0,0 }, { 1,0,0,0 }},
                {{ 0,0,0,0 }, { 1,1,1,0 }, { 0,0,1,0 }, { 0,0,0,0 }},
                {{ 0,1,0,0 }, { 0,1,0,0 }, { 1,1,0,0 }, { 0,0,0,0 }}
        },
        { // J
                {{ 0,0,0,0 }, { 0,0,1,0 }, { 1,1,1,0 }, { 0,0,0,0 }},
                {{ 1,0,0,0 }, { 1,0,0,0 }, { 1,1,0,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 1,1,1,0 }, { 1,0,0,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 1,1,0,0 }, { 0,1,0,0 }, { 0,1,0,0 }}
        },
        { // O
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }},
                {{ 0,0,0,0 }, { 0,1,1,0 }, { 0,1,1,0 }, { 0,0,0,0 }}
        }
};

int board[BOARD_HEIGHT][BOARD_WIDTH];

void _shuffle(int* arr, int size) {
  for (int i = size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

int* generate_7bag() {
  int* blocks = malloc(sizeof(int) * 7);
  for (int i = 0; i < 7; i++)
    blocks[i] = i;
  _shuffle(blocks, 7);
  return blocks;
}

int blockIdx = 0;
int* blocks = NULL;
int* nextBlocks = NULL;
int rotation = 0;
int x = 0, y = 0;

int* get_next_blocks() {
  if (blocks != NULL)
    free(blocks);

  if (nextBlocks == NULL)
    nextBlocks = generate_7bag();

  blocks = nextBlocks;
  nextBlocks = generate_7bag();

  return blocks;
}

int get_next_block() {
  if (blockIdx == 0) {
    blocks = get_next_blocks();
  }

  int nextBlock = blocks[blockIdx];
  blockIdx = (blockIdx + 1) % 7;
  rotation = 0;
  x = y = 0;
  return nextBlock;
}

const char* blockColor[] = {
        FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE
};

int spawn_block() {
  int blockType = get_next_block();
  int (*blockData)[4] = block[blockType][rotation];

  int minY = 4, minX = 4, maxX = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (blockData[j][i]) {
        minY = min(minY, j);
        minX = min(minX, i);
        maxX = max(maxX, i);
      }
    }
  }

  int width = maxX - minX + 1;
  int startX = (BOARD_WIDTH - width) / 2 + 1;

  x = startX;
  y = 0;
  int code = draw_block();

  if (code == -1)
    return -1;
  return 0;
}

int control_block() {
  while (can_move_block(0, 1)) {
    int spaced = 0;
    for (int i = 0; i < 100 && !spaced; i++) {
      int key = read_key();
      switch (key) {
        case KEY_LEFT:
          move_block(-1, 0);
          break;
        case KEY_RIGHT:
          move_block(1, 0);
          break;
        case KEY_DOWN:
          move_block(0, 1);
          break;
        case 'z':
          rotate_block_left();
          break;
        case 'x':
          rotate_block_right();
          break;
        case ' ':
          drop_block();
          spaced = 1;
          break;
        case 'q':
          return -1;
      }

      if (!spaced)
        SLEEP(10);
    }

    if (!spaced)
      move_block(0, 1);
    else {
      break;
    }
  }

  fix_block();
  SLEEP(500);
  return 1;
}

int can_move_block(int dx, int dy) {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block[blocks[blockIdx]][rotation][j][i] == 1) {
        int nx = x + i + dx;
        int ny = y + j + dy;

        if (ny < 0 || ny >= BOARD_HEIGHT || nx < 0 || nx >= BOARD_WIDTH) {
          return 0;
        }

        if (board[ny][nx]) {
          return 0;
        }
      }
    }
  }

  return 1;
}

int can_rotate_left() {
  return 1;
}

int can_rotate_right() {
  return 1;
}

void fix_block() {
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block[blocks[blockIdx]][rotation][j][i]) {
        board[y + j][x + i] = blocks[blockIdx] + 1;
      }
    }
  }
}

int draw_block() {
  int ret = 0;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (block[blocks[blockIdx]][rotation][j][i]) {
        fill_block(x + i + 1, y + j, BLOCK_FILLED, blockColor[blocks[blockIdx]]);

        if (board[y + j + 1][x + i] != 0) {
          ret = -1;
        }
      }
    }
  }

  return ret;
}

void draw_next_block() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      move_cursor(2 + i, 6 + j);
      printf(" ");
    }
  }

  int nextBlockIdx = (blockIdx + 1) % 7;
  int nextBlock = blocks[nextBlockIdx];

  if (nextBlockIdx == 1) {
    nextBlock = nextBlocks[1];
  }

  int (*blockData)[4] = block[nextBlock][0];

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (blockData[j][i]) {
        move_cursor(2 + i, 6 + j);
        printf("%s%s%s", blockColor[nextBlock], BLOCK_FILLED, RESET);
      }
    }
  }
}

void erase_block() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (block[blocks[blockIdx]][rotation][j][i]) {
        fill_block(x + i + 1, y + j, BLOCK_EMPTY, RESET);
      }
    }
  }
}

void move_block(int dx, int dy) {
  if (can_move_block(dx, dy) == 0) {
    return;
  }

  erase_block();
  x += dx;
  y += dy;
  draw_block();
}

void rotate_block_left() {
  int newRotation = (rotation + 3) % 4;
  if (can_rotate_left() == 0) {
    return;
  }

  erase_block();
  rotation = newRotation;
  draw_block();
}

void rotate_block_right() {
  int newRotation = (rotation + 1) % 4;
  if (can_rotate_right() == 0) {
    return;
  }

  erase_block();
  rotation = newRotation;
  draw_block();
}

void drop_block() {
  while (can_move_block(0, 1)) {
    move_block(0, 1);
  }
  draw_block();
}

void clear_lines() {
  int changed = 0;
  for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
    int filled = 1;
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (board[i][j] == 0) {
        filled = 0;
        break;
      }
    }

    if (filled) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
          board[k][j] = board[k - 1][j];
        }
      }

      for (int j = 0; j < BOARD_WIDTH; j++) {
        board[0][j] = 0;
      }

      changed = 1;
      i++;
      score += 100;
    }
  }

  if (changed) {
    printf("\a");
    for (int i = 0; i < BOARD_HEIGHT; i++) {
      for (int j = 0; j < BOARD_WIDTH; j++) {
        if (board[i][j]) {
          fill_block(j + 1, i, BLOCK_FILLED, blockColor[board[i][j] - 1]);
        } else {
          fill_block(j + 1, i, BLOCK_EMPTY, RESET);
        }
      }
    }
  }
}

int main() {
  init_console();
  clear_console();
  init_game();

  while (1) {
    int code = spawn_block();
    draw_next_block();

    if (code == -1) break;

    code = control_block();
    clear_lines();

    if (code == -1) break;
  }

  end_game();

  return 0;
}
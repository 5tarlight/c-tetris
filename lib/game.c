#include "game.h"
#include "console.h"
#include <stdio.h>

void init_game() {
  srand(time(NULL));
  hide_cursor();
  draw_title();
  draw_board();
}

void end_game() {
  move_cursor(0, BOARD_HEIGHT + 1);
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

  // 다음 블록 표시
  move_cursor(0, 5);
  // C에서는 문자열을 이어붙일 때 자동으로 합쳐준다.
  printf("%s NEXT %s", BG_CYAN FG_BLACK, RESET);

  // 조작법
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

// https://strategywiki.org/wiki/Tetris/Rotation_systems
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
  // Fisher-Yates shuffle
  // https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
  // 0 ~ i 범위에서 랜덤한 수를 선택해서 swap한다.
  // 모든 순열이 나올 확률이 같다. (편향되지 않는다)
  // 시간 복잡도는 O(n)
  for (int i = size - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

// 7-bag
// https://tetris.fandom.com/wiki/Random_Generator
int* generate_7bag() {
  // static 변수는 매번 초기화되지 않고 프로그램이 종료될 때까지 유지된다.
  // 새로 만드는 비용을 줄일 수 있다.
  static int blocks[7] = { 0, 1, 2, 3, 4, 5, 6 };
  _shuffle(blocks, 7);
  return blocks;
}

int blockIdx = 0;
int* blocks = NULL;
int rotation = 0;

int get_next_block() {
  // 한바퀴 다 돌았거나 처음 시작할 때 새로 섞는다.
  if (blockIdx == 0) {
    blocks = generate_7bag();
  }

  int nextBlock = blocks[blockIdx];
  blockIdx = (blockIdx + 1) % 7;
  // 새로운 블록을 가져왔으므로 회전 상태를 초기화한다.
  rotation = 0;
  return nextBlock;
}

const char* blockColor[] = {
  FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE
};

void spawn_block() {
  int blockType = get_next_block();
  int (*blockData)[4] = block[blockType][rotation];

  int minY = 4, minX = 4, maxX = 0;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (blockData[y][x]) {
        minY = min(minY, y); // 블록의 최소 y값. 시작점 찾기
        minX = min(minX, x); // 블록의 최소 x값.
        maxX = max(maxX, x); // 블록의 최대 x값.
      }
    }
  }

  int width = maxX - minX + 1;
  int startX = (BOARD_WIDTH - width) / 2 + 1; // 중앙 정렬

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (blockData[y][x]) {
        fill_block(startX + x - minX, y - minY, BLOCK_FALLING, blockColor[blockType]);
      }
    }
  }
}

void control_block() {
  int key = read_key();

  while (can_move_block(0, 1)) {
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
      break;
    }
  
    if (can_move_block(0, 1)) {
      move_block(0, 1);
      draw_block();
      SLEEP(1000);
    }
  }

  fix_block();
}

int can_move_block(int dx, int dy) {
  int (*blockData)[4] = block[blocks[blockIdx]][rotation];

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if (blockData[y][x]) {
        int nx = x + dx;
        int ny = y + dy;

        if (ny < 0 || ny >= BOARD_HEIGHT || nx < 0 || nx >= BOARD_WIDTH) {
          return 0;
        }

        if (ny >= 0 && board[ny][nx]) {
          return 0;
        }
      }
    }
  }

  return 1;
}

int can_rotate_left() {
}

int can_rotate_right() {
}

void fix_block() {
}

void draw_block() {
}

void draw_next_block() {}

void move_block(int dx, int dy) {
}

void rotate_block_left() {
}

void rotate_block_right() {
}

void drop_block() {
}

void clear_lines() {
}

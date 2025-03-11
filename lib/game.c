#include "game.h"
#include "console.h"
#include <stdio.h>

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
  // 한바퀴 다 돌았거나 처음 시작할 때 새로 섞는다.
  if (blockIdx == 0) {
    blocks = get_next_blocks();
  }

  int nextBlock = blocks[blockIdx];
  blockIdx = (blockIdx + 1) % 7;
  // 새로운 블록을 가져왔으므로 회전 상태를 초기화한다.
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
        minY = min(minY, j); // 블록의 최소 y값. 시작점 찾기
        minX = min(minX, i); // 블록의 최소 x값.
        maxX = max(maxX, i); // 블록의 최대 x값.
      }
    }
  }

  int width = maxX - minX + 1;
  int startX = (BOARD_WIDTH - width) / 2 + 1; // 중앙 정렬

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
  // Erase first!
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

void print_board() {
  move_cursor(0, 20);
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      printf("%d", board[i][j]);
    }
    printf("\n");
  }
}
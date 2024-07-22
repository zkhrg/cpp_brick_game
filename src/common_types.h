#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

struct GameInfo {
  int height;
  int width;
  int** grid;
  int state;
};

enum class eGame { TETRIS, SNAKE };

#endif  // COMMON_TYPES_H

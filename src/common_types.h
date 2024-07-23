#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

enum class eGame { TETRIS, SNAKE };
enum class eCommonTypesState { GAMING, PAUSE, GAMEOVER };

struct GameInfo {
  int height;
  int width;
  int** grid;
  eCommonTypesState state;
};

#endif  // COMMON_TYPES_H

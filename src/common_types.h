#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

enum class eGame { TETRIS, SNAKE, NO_GAME };
enum class eCommonTypesState { GAMING, PAUSE, GAMEOVER };

struct GameInfo {
  int height;
  int width;
  int** grid;
  int next_fig;
  eCommonTypesState state;
};

#endif  // COMMON_TYPES_H

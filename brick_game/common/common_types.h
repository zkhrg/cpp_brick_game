#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

enum class eGame { TETRIS, SNAKE, NO_GAME };
enum class eCommonTypesState { GAMING, PAUSE, GAMEOVER };

struct GameInfo {
  int height;
  int width;
  int** grid;
  int next_fig;
  int figures_stats[7];
  int level;
  int points;
  int record;
  eCommonTypesState state;
};

#endif  // COMMON_TYPES_H

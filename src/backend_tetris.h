#ifndef BACKEND_TETRIS_H
#define BACKEND_TETRIS_H

#include <sys/time.h>

#include <random>

#include "arcade_game_base.h"

class TetrisGame : public ArcadeGame {
 public:
  TetrisGame();

  enum class eFigure { L, J, Z, S, T, I, O };
  enum class eGameState { START, PAUSE, GAMOVER, FINAL_STATE };
  enum class eDirection { LEFT, RIGHT };

  int tetramino_coords[4][2];
  int tetramino_corner[2];

  int prev_tetramino_coords[4][2];
  int prev_tetramino_corner[2];

  int field_height;
  int field_width;
  int* field_grid;

  int game_points;
  int game_speed;
  int game_tmp_speed;
  int game_pause;
  int game_highscore;
  long long game_last_tick_time;

  eFigure cur_figure;
  eFigure next_figure;

  eGameState game_state;

  int figures_stats[7];
  bool render_ready;
  bool stats_render_ready;
  bool values_render_ready;

  static constexpr const char FIGURES_COORDS[7][10] = {
      "040506144", "040506164", "040515164", "050614154",
      "040506154", "030405063", "040514153"};

  int level = 0;
  static const int empty_tile = 0;
  static const int tetramino_tile = 1;
  static const int filled_tile = 2;
  static const int figures_count = 7;
  static const int levels_count = 10;

  static constexpr const long long times[levels_count + 1] = {
      800, 690, 580, 470, 360, 250, 140, 100, 70, 45, 40};

  // random sh1t
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> distrib_;
  // the end of random sh1t

  void InitTetramino();
  void ClearPos();
  void AddTetraminoToField();
  void ApplyFall();
  void InitField(int h, int w);
  void MakeFallTetramino();
  bool IsTetraminoFell();
  void MoveTetramino(eDirection d);
  bool CanTetraminoMove(eDirection d);
  void FieldContentOneLineDown(int dest);
  void ClearGlassLine(int line);
  int CountFilledLines();
  void AddPoints();
  int GetFigureBoxSize(eFigure fig);
  bool CanFigureRotate(eFigure fig);
  void RotateFigure();
  void ApplySignal();
  bool IsTimeToTick();
  void MakeTick();
  long long GetTimeMs();
  void SetNewSpeed();
  void ReadHighscore();
  void WriteHighscore();
  bool CanAddTetramino();
  void TogglePause();
};

#endif  // BACKEND_TETRIS_H

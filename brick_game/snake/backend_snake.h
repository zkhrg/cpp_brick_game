#ifndef SNAKE_BACKEND_H
#define SNAKE_BACKEND_H
#include <sys/time.h>

#include <array>
#include <iostream>
#include <list>
#include <random>
#include <unordered_map>

#include "../common/arcade_game_base.h"

namespace s21 {
class SnakeGame : public ArcadeGame {
 public:
  enum class Direction { Up, Down, Left, Right };
  static inline std::unordered_map<Direction, std::pair<int, int>> m = {
      {Direction::Up, {-1, 0}},
      {Direction::Down, {1, 0}},
      {Direction::Left, {0, -1}},
      {Direction::Right, {0, 1}}};

 public:
  SnakeGame();
  ~SnakeGame();

 public:
  static inline std::array<int, 10> times = {180, 170, 160, 150, 140,
                                             130, 120, 110, 100, 90};
  int** field_grid_;
  int height_;
  int width_;
  int level;
  long long last_tick_time;
  int points;
  std::pair<int, int> food_ind;
  Direction dir;
  std::list<std::pair<int, int>>
      snake_body;  // y, x | y+ down, y- up | x+ right, x- left

  // random sh1t
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> distrib_;
  // the end of random sh1t

  void CreateField();
  void FreeField();
  int FindSpaceToSpawnApple(int ind);
  long long GetTimeMs();
  bool isTimeToTick();
  void AddSnakeToField();
  void MoveSnake();
  void GenerateAppleIndex();
  bool CheckFoodCollision();
  bool CheckStopCollision();
  void GrowSnake();
  void ClearField();
  void AddFoodToField();

 public:
  /* ======================================= overrided =========== */
  void MakeTick() override;
  void GetData(GameInfo& gi) const override;
  void HandleKey(eKeys k) override;
  /* ============================================================= */

  void PrintField();
  void set_direction(Direction val);
  int get_height() const;
  int get_width() const;
  enum class eState { GAMING, PAUSE, GAMEOVER };
  void SetState(eState s);
  eState GetState();
  eState state;
  bool ready_change_dir;

  void ReadHighscore();
  void WriteHighscore();
  int game_highscore;
};

}  // namespace s21
#endif

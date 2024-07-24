#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <Qt>
#include <memory>

#include "arcade_game_base.h"
#include "backend_snake.h"
#include "backend_tetris.h"
#include "common_types.h"
// #include "brickgame_backend.h"

class Controller {
 public:
  Controller();
  void SetGame(eGame g);
  void HandleKey(int aKey);
  void MakeTick();
  GameInfo GetData();

 public:
  eGame evgame;
  std::unique_ptr<ArcadeGame> current_game;
};

#endif  // CONTROLLER_H

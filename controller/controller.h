#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <memory>

#include "../brick_game/common/arcade_game_base.h"
#include "../brick_game/common/common_types.h"
#include "../brick_game/snake/backend_snake.h"
#include "../brick_game/tetris/backend_tetris.h"

class Controller {
 public:
  Controller();
  void SetGame(eGame g);
  void HandleKey(ArcadeGame::eKeys aKey);
  void MakeTick();
  void GetData(GameInfo &gi);

 public:
  eGame evgame;
  std::unique_ptr<ArcadeGame> current_game;
};

#endif  // CONTROLLER_H

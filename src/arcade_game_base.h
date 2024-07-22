#ifndef ARCADE_GAME_BASE_H
#define ARCADE_GAME_BASE_H

#include "common_types.h"

// чисто виртуальный класс
class ArcadeGame {
 public:
  enum class eKeys {
    Key_Up,
    Key_Down,
    Key_Left,
    Key_Right,
    Key_Space,
    Key_ESC
  };

 public:
  virtual ~ArcadeGame() = default;
  virtual void HandleKey(eKeys k) = 0;
  virtual GameInfo GetData() const = 0;
  virtual void MakeTick() = 0;
};

#endif  // ARCADE_GAME_BASE_H

#include "controller.h"

Controller::Controller() : current_game(nullptr) {}

void Controller::HandleKey(int aKey) {
  if (current_game == nullptr) return;
  // TODO: можно бы и заинициализировать эту k
  ArcadeGame::eKeys k = ArcadeGame::eKeys::Key_Space;
  switch (aKey) {
    case Qt::Key_Up: {
      k = ArcadeGame::eKeys::Key_Up;
      break;
    }
    case Qt::Key_Down: {
      k = ArcadeGame::eKeys::Key_Down;
      break;
    }
    case Qt::Key_Left: {
      k = ArcadeGame::eKeys::Key_Left;
      break;
    }
    case Qt::Key_Right: {
      k = ArcadeGame::eKeys::Key_Right;
      break;
    }
    case Qt::Key_Escape: {
      k = ArcadeGame::eKeys::Key_ESC;
      break;
    }
    default: {
      break;
    }
  }

  current_game->HandleKey(k);
}

void Controller::SetGame(eGame g) {
  evgame = g;
  switch (evgame) {
    case eGame::TETRIS:
      break;
    case eGame::SNAKE:
      current_game = std::make_unique<s21::SnakeGame>();
      break;
  }
}

GameInfo Controller::GetData() {
  current_game->MakeTick();
  if (current_game != nullptr) {
    return current_game->GetData();
  }
  return GameInfo{};
}

void Controller::MakeTick() { current_game->MakeTick(); }

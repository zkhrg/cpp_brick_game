#include "controller.h"

Controller::Controller() : current_game(nullptr) {}

void Controller::HandleKey(ArcadeGame::eKeys aKey) {
  if (current_game == nullptr) return;
  current_game->HandleKey(aKey);
}

void Controller::SetGame(eGame g) {
  evgame = g;
  switch (evgame) {
    case eGame::TETRIS:
      current_game = std::make_unique<s21::TetrisGame>();
      break;
    case eGame::SNAKE:
      current_game = std::make_unique<s21::SnakeGame>();
      break;
    case eGame::NO_GAME:
      current_game.reset();
      current_game = nullptr;
      break;
  }
}

void Controller::GetData(GameInfo &gi) {
  MakeTick();
  if (current_game != nullptr) {
    current_game->GetData(gi);
  }
}

void Controller::MakeTick() {
  if (current_game == nullptr) return;
  current_game->MakeTick();
}

#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QDir>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QTimer>

#include "controller.h"
#include "font_manager.h"
#include "texture_manager.h"

class glView : public QOpenGLWidget {
 public:
  glView();

  void initializeGL() override;
  void paintGL() override;
  void drawRectangle(int x1, int y1, QColor color);
  // void updateScene(GameInfo* gi);
  void DrawMenu();
  void KeyReleasedMenu(int aKey);
  void KeyPressedSnake(int aKey);
  void keyPressEvent(QKeyEvent* apKeyEvent) override;
  void keyPressedPauseMenu(int aKey);
  void keyPressedGameOverMenu(int aKey);

  void Processing();
  void DrawUsingState();

  void DrawSnakeGame();
  void DrawGameOverMenu();
  void DrawPauseMenu();
  void DrawTetrisGame();

  void SendDataToController();
  void GetDataFromController();
  void drawRectangleSprite(int x1, int y1, const QString& texturename, int dir);

 public:
  enum class eState { TETRIS, SNAKE, EXIT, MENU, PAUSE, GAMEOVER };
  enum class eMenu { TETRIS, SNAKE, EXIT };
  enum class ePauseMenu { RESUME, GO_TO_MAIN_MENU };
  enum class eGameOverMenu { PLAY_AGAIN, GO_TO_MAIN_MENU };

 private:
  int w;
  int h;
  int tile_size;
  std::vector<std::pair<eMenu, std::string>> mvMenu;
  std::vector<std::pair<ePauseMenu, std::string>> mvPauseMenu;
  eMenu mCurrentMenu{eMenu::TETRIS};
  ePauseMenu mCurrentPauseMenu{ePauseMenu::RESUME};
  eGameOverMenu mCurrentGameOverMenu{eGameOverMenu::PLAY_AGAIN};
  eState mState{eState::MENU};
  eState prevState{};
  Controller controller;
  QTimer* timer;
  TextureManager texture_manager;
  FontManager font_manager;
};

#endif // GLVIEW_HPP

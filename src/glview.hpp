#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#define GL_SILENCE_DEPRECATION

#include <QDir>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QTimer>

#include "controller.h"
#include "texture_manager.h"

class glView : public QOpenGLWidget {
 public:
  glView();

  void initializeGL() override;
  void paintGL() override;
  void drawRectangle(int x1, int y1);
  // void updateScene(GameInfo* gi);
  void DrawMenu();
  void KeyReleasedMenu(int aKey);
  void KeyPressedSnake(int aKey);
  void keyPressEvent(QKeyEvent* apKeyEvent) override;
  void Processing();
  void DrawUsingState();
  void DrawSnakeGame();
  void DrawTetrisGame();
  void SendDataToController();
  void GetDataFromController();
  void drawRectangleSprite(int x1, int y1, const QString& texturename, int dir);

 public:
  enum class eState { TETRIS, SNAKE, EXIT, MENU };
  enum class eMenu { TETRIS, SNAKE, EXIT };

 private:
  int w;
  int h;
  int tile_size;
  std::vector<std::pair<eMenu, std::string>> mvMenu;
  eMenu mCurrentMenu{eMenu::TETRIS};
  eState mState{eState::MENU};
  Controller controller;
  QTimer* timer;
  TextureManager texture_manager;
};

#endif // GLVIEW_HPP

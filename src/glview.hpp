#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QDir>
#include <QElapsedTimer>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QPainter>
#include <QTimer>
#include <array>
#include <sstream>

#include "controller.h"
#include "font_manager.h"
#include "texture_manager.h"

class glView : public QOpenGLWidget {
 public:
  glView();

  void initializeGL() override;
  void paintGL() override;
  void drawRectangle(int x1, int y1, QColor color);
  void resizeGL(int h, int w) override;
  // void updateScene(GameInfo* gi);
  void DrawMenu();
  void KeyPressedMenu(int aKey);
  void KeyPressedSnake(int aKey);
  void KeyPressedTetris(int aKey);
  void keyPressEvent(QKeyEvent* apKeyEvent) override;
  void KeyPressedPauseMenu(int aKey);
  void KeyPressedGameOverMenu(int aKey);

  GameInfo Processing();
  void DrawUsingState(GameInfo& gi);

  void DrawSnakeGame(GameInfo& gi);
  void DrawGameOverMenu();
  void DrawPauseMenu();
  void DrawTetrisGame(GameInfo& gi);

  void SendDataToController();
  void GetDataFromController();
  void drawRectangleSprite(int x1, int y1, const QString& texturename, int dir);

 public:
  enum class eState { TETRIS, SNAKE, EXIT, MENU, PAUSE, GAMEOVER };
  enum class eMenu { TETRIS, SNAKE, EXIT };
  enum class ePauseMenu { RESUME, GO_TO_MAIN_MENU };
  enum class eGameOverMenu { PLAY_AGAIN, GO_TO_MAIN_MENU };
  static inline std::array<QColor, 7> TetrisFiguresColors = {
      QColor(255, 255, 0),   QColor(0, 0, 255),     QColor(255, 0, 0),
      QColor(255, 192, 203), QColor(238, 130, 238), QColor(255, 165, 0),
      QColor(165, 42, 42)};

 private:
  int w;
  int h;
  int tile_size;
  std::vector<std::pair<eMenu, std::string>> mvMenu;
  std::vector<std::pair<ePauseMenu, std::string>> mvPauseMenu;
  std::vector<std::pair<eGameOverMenu, std::string>> mvGameOverMenu;

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

#define XS 1
#define YS 1
static const int figures_stats_coords[7][4][2] = {
    {{YS, XS}, {YS, XS + 1}, {YS, XS + 2}, {YS + 1, XS}},
    {{YS + 3, XS}, {YS + 3, XS + 1}, {YS + 3, XS + 2}, {YS + 4, XS + 2}},
    {{YS + 6, XS}, {YS + 6, XS + 1}, {YS + 7, XS + 1}, {YS + 7, XS + 2}},
    {{YS + 9, XS + 1}, {YS + 9, XS + 2}, {YS + 10, XS + 1}, {YS + 10, XS}},
    {{YS + 12, XS}, {YS + 12, XS + 1}, {YS + 12, XS + 2}, {YS + 13, XS + 1}},
    {{YS + 15, XS}, {YS + 15, XS + 1}, {YS + 16, XS}, {YS + 16, XS + 1}},
    {{YS + 18, XS}, {YS + 18, XS + 1}, {YS + 18, XS + 2}, {YS + 18, XS + 3}},
};

static const int PREVIEW_MASKS[7] = {
    0b000100011100, 0b010000011100, 0b011000001100, 0b001100011000,
    0b001000011100, 0b011110000000, 0b001100001100};

#endif // GLVIEW_HPP

#include "main_view.h"

/**************************************overrides***************************/

glView::eMenu operator++(glView::eMenu& aMenu) {
  if (aMenu == glView::eMenu::EXIT) {
    aMenu = glView::eMenu::TETRIS;
    return aMenu;
  }
  aMenu = glView::eMenu(int(aMenu) + 1);
  return aMenu;
}

glView::eMenu operator--(glView::eMenu& aMenu) {
  if (aMenu == glView::eMenu::TETRIS) {
    aMenu = glView::eMenu::EXIT;
    return aMenu;
  }
  aMenu = glView::eMenu(int(aMenu) - 1);
  return aMenu;
}

glView::ePauseMenu operator++(glView::ePauseMenu& aPauseMenu) {
  if (aPauseMenu == glView::ePauseMenu::GO_TO_MAIN_MENU) {
    aPauseMenu = glView::ePauseMenu::RESUME;
    return aPauseMenu;
  }
  aPauseMenu = glView::ePauseMenu(int(aPauseMenu) + 1);
  return aPauseMenu;
}

glView::ePauseMenu operator--(glView::ePauseMenu& aPauseMenu) {
  if (aPauseMenu == glView::ePauseMenu::RESUME) {
    aPauseMenu = glView::ePauseMenu::GO_TO_MAIN_MENU;
    return aPauseMenu;
  }
  aPauseMenu = glView::ePauseMenu(int(aPauseMenu) - 1);
  return aPauseMenu;
}

glView::eGameOverMenu operator++(glView::eGameOverMenu& aGameOverMenu) {
  if (aGameOverMenu == glView::eGameOverMenu::GO_TO_MAIN_MENU) {
    aGameOverMenu = glView::eGameOverMenu::PLAY_AGAIN;
    return aGameOverMenu;
  }
  aGameOverMenu = glView::eGameOverMenu(int(aGameOverMenu) + 1);
  return aGameOverMenu;
}

glView::eGameOverMenu operator--(glView::eGameOverMenu& aGameOverMenu) {
  if (aGameOverMenu == glView::eGameOverMenu::PLAY_AGAIN) {
    aGameOverMenu = glView::eGameOverMenu::GO_TO_MAIN_MENU;
    return aGameOverMenu;
  }
  aGameOverMenu = glView::eGameOverMenu(int(aGameOverMenu) - 1);
  return aGameOverMenu;
}

/**************************************************************************/

glView::glView() : timer(new QTimer(this)) {
  connect(timer, &QTimer::timeout, this, &glView::paintGL);
  timer->start(1000 / 60);
  setWindowTitle("brick game");
  gi = {};
  w = 400;
  h = 800;
  tile_size = w / 10;
  tiles_count_w = w / tile_size;
  tiles_count_h = h / tile_size;
  InitGridAccepter();
  setFixedSize(w, h);

  font_manager.loadFont("jetbrains_regular",
                        ":/fonts/JetBrainsMono-Regular.ttf");
  font_manager.loadFont("jetbrains_bold", ":/fonts/JetBrainsMono-Bold.ttf");

  mvMenu.push_back({eMenu::TETRIS, "Tetris"});
  mvMenu.push_back({eMenu::SNAKE, "Snake"});
  mvMenu.push_back({eMenu::EXIT, "Exit"});

  mvPauseMenu.push_back({ePauseMenu::RESUME, "Resume"});
  mvPauseMenu.push_back({ePauseMenu::GO_TO_MAIN_MENU, "Return to menu"});

  mvGameOverMenu.push_back({eGameOverMenu::PLAY_AGAIN, "Play again"});
  mvGameOverMenu.push_back({eGameOverMenu::GO_TO_MAIN_MENU, "Return to menu"});
}

glView::~glView() {
  for (int i = 0; i < gi.height; i++) {
    delete[] gi.grid[i];
  }
  delete[] gi.grid;
}

void glView::initializeGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, -1, 1);

  texture_manager.loadTexture("snakehead", ":/images/snakehead.png");
  texture_manager.loadTexture("snakebody", ":/images/snakebody.png");
}

void glView::paintGL() {
  glClearColor(0.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  Processing();
  DrawUsingState();
  update();
}

void glView::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  QMatrix4x4 projection;
  projection.ortho(0, w, h, 0, -1, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projection.constData());
}

// нужно будет как-то еще передавать сюда цвет
void glView::drawRectangle(int x1, int y1, QColor color) {
  glBegin(GL_QUADS);
  glColor3f(color.redF(), color.greenF(), color.blueF());

  glVertex2i(x1, y1);
  glVertex2i(x1 + tile_size, y1);
  glVertex2i(x1 + tile_size, y1 + tile_size);
  glVertex2i(x1, y1 + tile_size);

  glEnd();
}

void glView::drawRectangleSprite(int x1, int y1, const QString &texturename,
                                 int rotation) {
  QOpenGLTexture *texture = texture_manager.getTexture(texturename);
  if (!texture) return;

  texture->bind();
  // белый фон под картинку чтобы цвета не менялись
  glColor3f(1.0f, 1.0f, 1.0f);

  glEnable(GL_TEXTURE_2D);

  GLfloat texCoords[4][4][2] = {
      {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}},  // 0
      {{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}},  // 90
      {{1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}},  // 180
      {{0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}}   // 270
  };

  int r = rotation % 4;

  glBegin(GL_QUADS);
  for (int i = 0; i < 4; ++i) {
    glTexCoord2f(texCoords[r][i][0], texCoords[r][i][1]);
    switch (i) {
      case 0:
        glVertex2i(x1, y1);
        break;
      case 1:
        glVertex2i(x1 + tile_size, y1);
        break;
      case 2:
        glVertex2i(x1 + tile_size, y1 + tile_size);
        break;
      case 3:
        glVertex2i(x1, y1 + tile_size);
        break;
    }
  }
  glEnd();

  glDisable(GL_TEXTURE_2D);
  texture->release();
}

void glView::DrawUsingState() {
  switch (mState) {
    case eState::MENU:
      DrawMenu();
      break;
    case eState::SNAKE:
      DrawSnakeGame();
      break;
    case eState::TETRIS:
      DrawTetrisGame();
      break;
    case eState::PAUSE:
      DrawPauseMenu();
      break;
    case eState::GAMEOVER:
      DrawGameOverMenu();
      break;
    case eState::EXIT:
      break;
  }
}

void glView::DrawMenu() {
  setWindowTitle("Brick Game | Main Menu");
  static auto app_w = w / 2.f;
  static auto app_h = h / 3.f;

  static auto font = font_manager.getFont("jetbrains_regular");
  static auto font_selected = font_manager.getFont("jetbrains_bold");

  font.setPointSize(70);
  font_selected.setPointSize(70);
  font_selected.setBold(true);

  QPainter painter(this);

  auto x = app_w - 135;
  auto y = app_h;
  auto dy = 100.f;
  int end = mvMenu.size();

  for (int i = 0; i < end; ++i) {
    if (i == (int)mCurrentMenu) {
      painter.setPen(Qt::red);
      painter.setFont(font_selected);
      painter.drawText(x, y, mvMenu[i].second.c_str());
    } else {
      painter.setPen(Qt::gray);
      painter.setFont(font);
      painter.drawText(x, y, mvMenu[i].second.c_str());
    }
    y += dy;
  }
}

void glView::Processing() {
  controller.GetData(gi);
  if (gi.state == eCommonTypesState::GAMEOVER) {
    controller.SetGame(eGame::NO_GAME);
    gi.state = eCommonTypesState::GAMING;
    prevState = mState;
    mState = eState::GAMEOVER;
  }
  if (mState == eState::EXIT) {
    close();
  }
}

void glView::DrawSnakeGame() {
  int transitions[4] = {0, 2, 1, 3};
  std::ostringstream oss;
  oss << "Snake Game | Points: " << std::setw(3) << gi.points
      << " | Level: " << std::setw(3) << gi.level
      << " | Highscore: " << std::setw(3) << gi.highscore;
  std::string title = oss.str();

  setWindowTitle(QString::fromStdString((title)));
  for (int i = 0; i < gi.height; i++) {
    for (int j = 0; j < gi.width; j++) {
      if (gi.grid[i][j] == 1) {
        drawRectangleSprite(j * tile_size, i * tile_size, "snakebody", 0);
      } else if (gi.grid[i][j] == 2) {
        drawRectangle(j * tile_size, i * tile_size, Qt::magenta);
      } else if (gi.grid[i][j] < 0) {
        drawRectangleSprite(j * tile_size, i * tile_size, "snakehead",
                            transitions[-(gi.grid[i][j] + 1)]);
      }
    }
  }
}

void glView::DrawTetrisGame() {
  QString s = QString("Tetris Game | Points: %1 | Level: %2")
                  .arg(gi.points, 3)
                  .arg(gi.level, 3);

  setWindowTitle(s);
  setFixedSize(w * 3 + 1 * tile_size, h);
  int start = w;

  for (int i = 0; i < gi.height; i++) {
    for (int j = 0; j < gi.width; j++) {
      if (gi.grid[i][j] > 0) {
        drawRectangle(j * tile_size + start, i * tile_size,
                      TetrisFiguresColors[(gi.grid[i][j] - 1) % 7]);
      }
    }
  }

  for (int i = 0; i < gi.height; i++) {
    drawRectangle(start - tile_size, i * tile_size, Qt::black);
    drawRectangle((start * 2), i * tile_size, Qt::black);
  }
  // ya ne uspevayu sdelat' tut otobrazhenie счета
  // for (int i = 0; i < 7; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     int k = i;
  //     if (k == 6) {
  //       k = 5;
  //     } else if (k == 5) {
  //       k = 6;  // yandex-style code | shout out kostik
  //     }
  //     drawRectangle(figures_stats_coords[i][j][1] * tile_size,
  //                   figures_stats_coords[i][j][0] * tile_size,
  //                   TetrisFiguresColors[k % 7]);
  //   }
  // }

  int bits = PREVIEW_MASKS[gi.next_fig];
  int start_j = w * 2 + tile_size * 3;
  int start_i = 2 * tile_size;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      if (bits & 1) {
        drawRectangle(start_j + j * tile_size, start_i + i * tile_size,
                      TetrisFiguresColors[gi.next_fig % 7]);
      }
      bits >>= 1;
    }
  }

  start_j = w * 2 + tile_size * 2;

  for (int i = 1; i < 4; i++) {
    drawRectangle(start_j, i * tile_size, Qt::black);
    drawRectangle(start_j + 7 * tile_size, i * tile_size, Qt::black);
  }

  for (int j = start_j / tile_size; j <= start_j / tile_size + 7; j++) {
    drawRectangle(j * tile_size, 1 * tile_size, Qt::black);
    drawRectangle(j * tile_size, 4 * tile_size, Qt::black);
  }
}

void glView::KeyPressedSnake(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Down: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Left: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Right: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
      // TODO:
    case Qt::Key_Escape: {
      controller.HandleKey(ConvertKey(aKey));
      prevState = mState;
      mState = eState::PAUSE;
      break;
    }
  }
}

void glView::KeyPressedTetris(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Down: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Left: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
    case Qt::Key_Right: {
      controller.HandleKey(ConvertKey(aKey));
      break;
    }
      // TODO:
    case Qt::Key_Escape: {
      setFixedSize(w, h);
      controller.HandleKey(ConvertKey(aKey));
      prevState = mState;
      mState = eState::PAUSE;
      break;
    }
  }
}

void glView::KeyPressedPauseMenu(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      --mCurrentPauseMenu;
      break;
    }
    case Qt::Key_Down: {
      ++mCurrentPauseMenu;
      break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return: {
      if (mCurrentPauseMenu == ePauseMenu::RESUME) {
        mState = prevState;
        controller.HandleKey(ConvertKey(Qt::Key_Escape));
      } else {
        controller.SetGame(eGame::NO_GAME);
        mState = eState::MENU;
      }
      break;
    }
  }
}

void glView::KeyPressedGameOverMenu(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      --mCurrentGameOverMenu;
      break;
    }
    case Qt::Key_Down: {
      ++mCurrentGameOverMenu;
      break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return: {
      if (mCurrentGameOverMenu == eGameOverMenu::PLAY_AGAIN) {
        mState = prevState;

        if (mState == eState::SNAKE) {
          controller.SetGame(eGame::SNAKE);
        }
        if (mState == eState::TETRIS) controller.SetGame(eGame::TETRIS);
      } else {
        controller.SetGame(eGame::NO_GAME);
        mState = eState::MENU;
      }
      break;
    }
  }
}

void glView::keyPressEvent(QKeyEvent* apKeyEvent) {
  switch (mState) {
    case eState::MENU:
      KeyPressedMenu(apKeyEvent->key());
    case eState::TETRIS:
      KeyPressedTetris(apKeyEvent->key());
      break;
    case eState::SNAKE:
      KeyPressedSnake(apKeyEvent->key());
      break;
    case eState::PAUSE:
      KeyPressedPauseMenu(apKeyEvent->key());
      break;
    case eState::GAMEOVER:
      KeyPressedGameOverMenu(apKeyEvent->key());
      break;
    case eState::EXIT:
      break;
  }
}

void glView::KeyPressedMenu(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      --mCurrentMenu;
      break;
    }
    case Qt::Key_Down: {
      ++mCurrentMenu;
      break;
    }
    case Qt::Key_Enter:
    case Qt::Key_Return: {
      mState = (eState)mCurrentMenu;
      if (mState == eState::SNAKE) controller.SetGame(eGame::SNAKE);
      if (mState == eState::TETRIS) controller.SetGame(eGame::TETRIS);

      break;
    }
  }
}

void glView::DrawPauseMenu() {
  setWindowTitle("Brick Game | Pause");
  setFixedSize(w, h);

  static auto app_w = w / 2.f;
  static auto app_h = h / 3.f;

  static auto font = font_manager.getFont("jetbrains_regular");
  static auto font_selected = font_manager.getFont("jetbrains_bold");

  font.setPointSize(50);
  font_selected.setPointSize(50);
  font_selected.setBold(true);

  QPainter painter(this);

  auto x = app_w - 135;
  auto y = app_h;
  auto dy = 80.f;
  int end = mvPauseMenu.size();

  for (int i = 0; i < end; ++i) {
    if (i == (int)mCurrentPauseMenu) {
      painter.setPen(Qt::red);
      painter.setFont(font_selected);
      painter.drawText(x, y, mvPauseMenu[i].second.c_str());
    } else {
      painter.setPen(Qt::gray);
      painter.setFont(font);
      painter.drawText(x, y, mvPauseMenu[i].second.c_str());
    }
    y += dy;
  }
}

void glView::DrawGameOverMenu() {
  setWindowTitle("Brick Game | Game Over");
  setFixedSize(w, h);

  static auto app_w = w / 2.f;
  static auto app_h = h / 3.f;

  static auto font = font_manager.getFont("jetbrains_regular");
  static auto font_selected = font_manager.getFont("jetbrains_bold");

  font.setPointSize(50);
  font_selected.setPointSize(50);
  font_selected.setBold(true);

  QPainter painter(this);

  auto x = app_w - 135;
  auto y = app_h;
  auto dy = 80.f;
  int end = mvGameOverMenu.size();

  for (int i = 0; i < end; ++i) {
    if (i == (int)mCurrentGameOverMenu) {
      painter.setPen(Qt::red);
      painter.setFont(font_selected);
      painter.drawText(x, y, mvGameOverMenu[i].second.c_str());
    } else {
      painter.setPen(Qt::gray);
      painter.setFont(font);
      painter.drawText(x, y, mvGameOverMenu[i].second.c_str());
    }
    y += dy;
  }
}

ArcadeGame::eKeys glView::ConvertKey(int aKey) {
  ArcadeGame::eKeys k = ArcadeGame::eKeys::Key_Space;
  auto it = KeyMap.find((Qt::Key)aKey);
  if (it != KeyMap.end()) {
    k = it->second;
  }
  return k;
}

void glView::InitGridAccepter() {
  gi.grid = new int*[tiles_count_h]();
  for (int i = 0; i < tiles_count_h; ++i) {
    gi.grid[i] = new int[tiles_count_w]();
  }
}

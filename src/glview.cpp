#include "glview.hpp"

glView::eMenu operator++(glView::eMenu &aMenu) {
  if (aMenu == glView::eMenu::EXIT) {
    aMenu = glView::eMenu::TETRIS;
    return aMenu;
  }
  aMenu = glView::eMenu(int(aMenu) + 1);
  return aMenu;
}

glView::eMenu operator--(glView::eMenu &aMenu) {
  if (aMenu == glView::eMenu::TETRIS) {
    aMenu = glView::eMenu::EXIT;
    return aMenu;
  }
  aMenu = glView::eMenu(int(aMenu) - 1);
  return aMenu;
}

glView::glView() : timer(new QTimer(this)) {
  connect(timer, &QTimer::timeout, this, &glView::paintGL);
  timer->start(1000 / 60);
  setWindowTitle("brick game");
  w = 450;
  h = 900;
  tile_size = w / 10;
  setFixedSize(w, h);

  font_manager.loadFont("jetbrains_regular",
                        ":/fonts/JetBrainsMono-Regular.ttf");
  font_manager.loadFont("jetbrains_bold", ":/fonts/JetBrainsMono-Bold.ttf");

  mvMenu.push_back({eMenu::TETRIS, "Tetris"});
  mvMenu.push_back({eMenu::SNAKE, "Snake"});
  mvMenu.push_back({eMenu::EXIT, "Exit"});
}

void glView::initializeGL() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, w, h, 0, 0, 1);

  texture_manager.loadTexture("snakehead", ":/images/snakehead.png");
  texture_manager.loadTexture("snakebody", ":/images/snakebody.png");
}

void glView::paintGL() {
  glClearColor(0.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  Processing();
  DrawUsingState();
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
    case eState::EXIT:
      break;
  }
}

void glView::DrawMenu() {
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

void glView::KeyReleasedMenu(int aKey) {
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
      break;
    }
  }
  update();
}

void glView::KeyPressedSnake(int aKey) {
  switch (aKey) {
    case Qt::Key_Up: {
      controller.HandleKey(aKey);
      break;
    }
    case Qt::Key_Down: {
      controller.HandleKey(aKey);
      break;
    }
    case Qt::Key_Left: {
      controller.HandleKey(aKey);
      break;
    }
    case Qt::Key_Right: {
      controller.HandleKey(aKey);
      break;
    }
      // TODO:
    case Qt::Key_Escape: {
      controller.HandleKey(aKey);
      break;
    }
  }
}

void glView::keyPressEvent(QKeyEvent *apKeyEvent) {
  switch (mState) {
    case eState::MENU:
      KeyReleasedMenu(apKeyEvent->key());
    case eState::TETRIS:
      break;
    case eState::SNAKE:
      KeyPressedSnake(apKeyEvent->key());
      break;
    case eState::EXIT:
      break;
  }
}

void glView::Processing() {
  switch ((int)mState) {
    case (int)eState::TETRIS: {
      // можно создавать экземпляр а потом его грохать
      break;
    }
    case (int)eState::SNAKE: {
      // можно создавать экземпляр а потом его грохать
      // TODO: rework
      if (controller.current_game == nullptr) controller.SetGame(eGame::SNAKE);
      break;
    }
    case (int)eState::EXIT: {
      close();
      break;
    }
  }
}

// Условно нажатия в контороллер пушит фронт
// Фронт так же оправшивает контроллер постоянно
// (с условно минимальной задержкой)
void glView::DrawSnakeGame() {
  GameInfo gt = controller.GetData();
  if (gt.state == eCommonTypesState::PAUSE) {
    // render_pause()
    // set pause at view
  } else if (gt.state == eCommonTypesState::GAMEOVER) {
    // render_gameo()
    // set gamoever at view
    // TODO: may вынести в процессинг чтобы в драу у меня лежала только игра.
  }
  for (int i = 0; i < gt.height; i++) {
    for (int j = 0; j < gt.width; j++) {
      switch (gt.grid[i][j]) {
        case 1: {
          drawRectangleSprite(j * tile_size, i * tile_size, "snakebody", 0);
          break;
        }
        case 2: {
          drawRectangle(j * tile_size, i * tile_size, Qt::magenta);
          break;
        }
        // TODO: rework
        case -1: {
          drawRectangleSprite(j * tile_size, i * tile_size, "snakehead", 0);
          break;
        }
        case -2: {
          drawRectangleSprite(j * tile_size, i * tile_size, "snakehead", 2);
          break;
        }
        case -3: {
          drawRectangleSprite(j * tile_size, i * tile_size, "snakehead", 1);
          break;
        }
        case -4: {
          drawRectangleSprite(j * tile_size, i * tile_size, "snakehead", 3);
          break;
        }
      }
    }
  }
  for (int i = 0; i < gt.height; ++i) {
    delete[] gt.grid[i];
  }
  delete[] gt.grid;

  update();
}

void glView::DrawTetrisGame() {
  ;
  ;
}

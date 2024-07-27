#include "main_view.h"

/**************************************************************************/

cliView::eMenu operator++(cliView::eMenu& aMenu) {
  if (aMenu == cliView::eMenu::EXIT) {
    aMenu = cliView::eMenu::TETRIS;
    return aMenu;
  }
  aMenu = cliView::eMenu(int(aMenu) + 1);
  return aMenu;
}

cliView::eMenu operator--(cliView::eMenu& aMenu) {
  if (aMenu == cliView::eMenu::TETRIS) {
    aMenu = cliView::eMenu::EXIT;
    return aMenu;
  }
  aMenu = cliView::eMenu(int(aMenu) - 1);
  return aMenu;
}

cliView::ePauseMenu operator++(cliView::ePauseMenu& aPauseMenu) {
  if (aPauseMenu == cliView::ePauseMenu::GO_TO_MAIN_MENU) {
    aPauseMenu = cliView::ePauseMenu::RESUME;
    return aPauseMenu;
  }
  aPauseMenu = cliView::ePauseMenu(int(aPauseMenu) + 1);
  return aPauseMenu;
}

cliView::ePauseMenu operator--(cliView::ePauseMenu& aPauseMenu) {
  if (aPauseMenu == cliView::ePauseMenu::RESUME) {
    aPauseMenu = cliView::ePauseMenu::GO_TO_MAIN_MENU;
    return aPauseMenu;
  }
  aPauseMenu = cliView::ePauseMenu(int(aPauseMenu) - 1);
  return aPauseMenu;
}

cliView::eGameOverMenu operator++(cliView::eGameOverMenu& aGameOverMenu) {
  if (aGameOverMenu == cliView::eGameOverMenu::GO_TO_MAIN_MENU) {
    aGameOverMenu = cliView::eGameOverMenu::PLAY_AGAIN;
    return aGameOverMenu;
  }
  aGameOverMenu = cliView::eGameOverMenu(int(aGameOverMenu) + 1);
  return aGameOverMenu;
}

cliView::eGameOverMenu operator--(cliView::eGameOverMenu& aGameOverMenu) {
  if (aGameOverMenu == cliView::eGameOverMenu::PLAY_AGAIN) {
    aGameOverMenu = cliView::eGameOverMenu::GO_TO_MAIN_MENU;
    return aGameOverMenu;
  }
  aGameOverMenu = cliView::eGameOverMenu(int(aGameOverMenu) - 1);
  return aGameOverMenu;
}

/**************************************************************************/

cliView::cliView() {
  initscr();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(30);
  if (has_colors() == FALSE || can_change_color() == FALSE) {
    endwin();
    printf("Your terminal does not support required color features\n");
  }
  start_color();
  init_color((int)Colors::YELLOW, 1000, 1000, 0);
  init_color((int)Colors::BLUE, 0, 0, 1000);
  init_color((int)Colors::RED, 1000, 0, 0);
  init_color((int)Colors::PINK, 1000, 700, 800);
  init_color((int)Colors::VIOLET, 500, 0, 500);
  init_color((int)Colors::ORANGE, 1000, 647, 0);
  init_color((int)Colors::BROWN, 647, 164, 164);
  init_pair(1, COLOR_WHITE, (int)Colors::YELLOW);
  init_pair(2, COLOR_WHITE, (int)Colors::BLUE);
  init_pair(3, COLOR_WHITE, (int)Colors::RED);
  init_pair(4, COLOR_WHITE, (int)Colors::PINK);
  init_pair(5, COLOR_WHITE, (int)Colors::VIOLET);
  init_pair(6, COLOR_WHITE, (int)Colors::ORANGE);
  init_pair(7, COLOR_WHITE, (int)Colors::BROWN);
  init_pair(8, COLOR_WHITE, COLOR_WHITE); /*bordercolor*/
  init_pair(9, COLOR_BLACK, COLOR_WHITE); /*textinborder*/
  // init_pair(10, COLOR_WHITE, COLOR_BLACK);

  mState = eState::MENU;

  mvMenu.push_back({eMenu::TETRIS, "Tetris"});
  mvMenu.push_back({eMenu::SNAKE, "Snake"});
  mvMenu.push_back({eMenu::EXIT, "Exit"});

  mvPauseMenu.push_back({ePauseMenu::RESUME, "Resume"});
  mvPauseMenu.push_back({ePauseMenu::GO_TO_MAIN_MENU, "Menu"});

  mvGameOverMenu.push_back({eGameOverMenu::PLAY_AGAIN, "Retry"});
  mvGameOverMenu.push_back({eGameOverMenu::GO_TO_MAIN_MENU, "Menu"});

  controller = {};

  main_w = subwin(stdscr, 20, 10 * kMult, 1, 12 * kMult);
  preview_next_figure_w = subwin(stdscr, 2, 6 * kMult, 3, 25 * kMult);
  score_w = subwin(stdscr, 2, 6 * kMult, 7, 25 * kMult);
  highscore_w = subwin(stdscr, 2, 6 * kMult, 10, 25 * kMult);
  level_w = subwin(stdscr, 2, 6 * kMult, 13, 25 * kMult);
  figures_stats_w = subwin(stdscr, 20, 10 * kMult, 1, 1 * kMult);
  gi = {};
  tiles_count_h = 20;
  tiles_count_w = 10;
  InitGridAccepter();
}

void cliView::InitGridAccepter() {
  gi.grid = new int*[tiles_count_h]();
  for (int i = 0; i < tiles_count_h; ++i) {
    gi.grid[i] = new int[tiles_count_w]();
  }
}

void cliView::PrintRectangle(int x1, int y1, int x2, int y2) {
  int left_x = std::min(x1, x2);
  int right_x = std::max(x1, x2);
  int bottom_y = std::max(y1, y2);
  int top_y = std::min(y1, y2);

  for (int i = top_y; i < bottom_y; i++) {
    for (int x = left_x; x < left_x + 1 * kMult; x++) {
      mvaddch(i, x, full_block | COLOR_PAIR(8));
    }
    for (int x = right_x; x < right_x + 1 * kMult; x++) {
      mvaddch(i, x, full_block | COLOR_PAIR(8));
    }
  }

  for (int i = left_x; i < right_x + 1 * kMult; i++) {
    mvaddch(top_y, i, full_block | COLOR_PAIR(8));
    mvaddch(bottom_y, i, full_block | COLOR_PAIR(8));
  }
}

void cliView::DrawOverlay() {
  PrintRectangle(0 * kMult, 0, 11 * kMult, 21);   // stats
  PrintRectangle(11 * kMult, 0, 22 * kMult, 21);  // main
  PrintRectangle(22 * kMult, 0, 33 * kMult, 21);  // left
  PrintRectangle(24 * kMult, 2, 31 * kMult, 5);   // next_box
  wattr_on(stdscr, COLOR_PAIR(9), NULL);
  mvwaddstr(stdscr, 0, 1 * kMult, "statistics");
  mvwaddstr(stdscr, 2, 25 * kMult, "next");
  wattr_off(stdscr, COLOR_PAIR(9), NULL);
}

ArcadeGame::eKeys cliView::ConvertKey(int aKey) {
  ArcadeGame::eKeys k = ArcadeGame::eKeys::Key_Space;
  auto it = KeyMap.find(aKey);
  if (it != KeyMap.end()) {
    k = it->second;
  }
  return k;
}

void cliView::RenderNextFigurePreview(int fig) {
  int bits = preview_masks[fig];

  wclear(preview_next_figure_w);
  wrefresh(preview_next_figure_w);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 6; j++) {
      if (bits & 1) {
        for (int x = j * kMult; x < j * kMult + (1 * kMult); x++) {
          mvwaddch(preview_next_figure_w, i, x,
                   full_block | COLOR_PAIR((fig % tetris_figures_count) + 1));
        }
      }
      bits >>= 1;
    }
  }
  wrefresh(preview_next_figure_w);
}

void cliView::RenderField() {
  if (tp->game_state == pause_state) {
    RenderPause();
    return;
  } else if (tp->game_state == gameover_state) {
    RenderGameOver();
    return;
  }

  wclear(main_w);
  for (int i = 0; i < tp->field_height; i++) {
    for (int j = 0; j < tp->field_width; j++) {
      if ((tp->field_grid[i * tp->field_width + j] > 0)) {
        for (int x = j * MULT; x < j * MULT + (1 * MULT); x++) {
          mvwaddch(s, i, x,
                   EMPTY_BLOCK |
                       COLOR_PAIR(((tp->field_grid[i * tp->field_width + j] -
                                    TETRAMINO_TILE) %
                                   FIGURES_COUNT) +
                                  TETRAMINO_TILE));
        }
      }
    }
  }

  tp->render_ready = 0;
  wrefresh(main_w);
}

void cliView::RenderPoints() {
  char str[64];
  wclear(score_w);
  sprintf(str, "%d", gi.points);
  mvwaddstr(score_w, 0, 0, "points");
  mvwaddstr(score_w, 1, 0, str);
  wrefresh(score_w);
}

void cliView::RenderHighscore() {
  char str[64];
  wclear(highscore_w);
  sprintf(str, "%d", gi.highscore);
  mvwaddstr(highscore_w, 0, 0, "record");
  mvwaddstr(highscore_w, 1, 0, str);
  wrefresh(highscore_w);
}

void cliView::RenderLevel() {
  char str[64];
  wclear(level_w);
  sprintf(str, "%d", gi.level);
  mvwaddstr(level_w, 0, 0, "level");
  mvwaddstr(level_w, 1, 0, str);
  wrefresh(level_w);
}

void cliView::RenderStats() {
  for (int i = 0; i < tetris_figures_count; i++) {
    for (int j = 0; j < 4; j++) {
      for (int x = figures_stats_coords[i][j][1] * kMult;
           x < figures_stats_coords[i][j][1] * kMult + 1 * kMult; x++) {
        int k = i;
        if (k == 6) {
          k = 5;
        } else if (k == 5) {
          k = 6;  // yandex-style code | shout out kostik
        }
        mvwaddch(figures_stats_w, figures_stats_coords[i][j][0], x,
                 full_block | COLOR_PAIR(k + 1));
      }
    }
  }
  wrefresh(figures_stats_w);
}

void cliView::RenderStatsValues() {
  int mult = 3;
  if (gi.state != eCommonTypesState::PAUSE) {
    for (int i = 0; i < tetris_figures_count; i++) {
      int tmp_i = i;
      if (i == 6)
        i = 5;
      else if (i == 5)
        i = 6;
      char str[64];
      sprintf(str, "%03d", gi.figures_stats[tmp_i]);
      mvwaddstr(figures_stats_w, (i * mult) + 1, 6 * kMult, str);
      i = tmp_i;
    }
  }
  gi.stats_render_ready = 0;
  wrefresh(figures_stats_w);
}

void cliView::RenderPause() {
  wclear(main_w);
  mvwaddstr(main_w, 9, 2 * kMult * kMult, "pause");
  wrefresh(main_w);
}

void cliView::RenderGameOver() {
  wclear(main_w);
  mvwaddstr(main_w, 9, 1 * kMult * kMult, "the end!");
  mvwaddstr(main_w, 10, 1 * kMult * kMult, "esc quit");
  wrefresh(main_w);
}

void cliView::RenderValues() {
  if (gi.state != eCommonTypesState::PAUSE) {
    RenderPoints();
    RenderHighscore();
    RenderLevel();
  }
  gi.values_render_ready = 0;
}

void cliView::DrawUsingState() {
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

void cliView::DrawMenu() {
  int y = 6;
  int x = 2 * kMult;
  auto dy = 1;
  int end = mvMenu.size();

  wclear(stdscr);

  PrintRectangle(0 * kMult, 0, 11 * kMult, 21);   // stats
  PrintRectangle(11 * kMult, 0, 22 * kMult, 21);  // main
  PrintRectangle(22 * kMult, 0, 33 * kMult, 21);  // left

  for (int i = 0; i < end; ++i) {
    if (i == (int)mCurrentMenu) {
      wattr_on(main_w, COLOR_PAIR(9), NULL);
      mvwaddstr(main_w, y, x, mvMenu[i].second.c_str());
      wattr_off(main_w, COLOR_PAIR(9), NULL);
    } else {
      mvwaddstr(main_w, y, x, mvMenu[i].second.c_str());
    }
    y += dy;
  }

  wrefresh(main_w);
}

void cliView::ApplyKeyUsingState(int key) {
  switch (mState) {
    case eState::MENU:
      KeyPressedMenu(key);
    case eState::TETRIS:
      KeyPressedTetris(key);
      break;
    case eState::SNAKE:
      KeyPressedSnake(key);
      break;
    case eState::PAUSE:
      KeyPressedPauseMenu(key);
      break;
    case eState::GAMEOVER:
      KeyPressedGameOverMenu(key);
      break;
    case eState::EXIT:
      break;
  }
}

void cliView::KeyPressedMenu(int key) {
  switch (key) {
    case KEY_UP: {
      --mCurrentMenu;
      break;
    }
    case KEY_DOWN: {
      ++mCurrentMenu;
      break;
    }
    // case KEY_ENTER:
    case '\n': {
      switch (mCurrentMenu) {
        case eMenu::SNAKE:
          mState = eState::SNAKE;
          controller.SetGame(eGame::SNAKE);
          break;
        case eMenu::TETRIS:
          mState = eState::TETRIS;
          controller.SetGame(eGame::TETRIS);
          break;
        case eMenu::EXIT:
          mState = eState::EXIT;
          break;
      }
      break;
    }
  }
}
void cliView::KeyPressedTetris(int key) {
  switch (key) {
    case KEY_UP: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_DOWN: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_LEFT: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_RIGHT: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case 27: {
      controller.HandleKey(ConvertKey(key));
      prevState = mState;
      mState = eState::PAUSE;
      break;
    }
  }
}
void cliView::KeyPressedSnake(int key) {
  switch (key) {
    case KEY_UP: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_DOWN: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_LEFT: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case KEY_RIGHT: {
      controller.HandleKey(ConvertKey(key));
      break;
    }
    case 27: {
      controller.HandleKey(ConvertKey(key));
      prevState = mState;
      mState = eState::PAUSE;
      break;
    }
  }
}
void cliView::KeyPressedPauseMenu(int key) {
  switch (key) {
    case KEY_UP: {
      --mCurrentPauseMenu;
      break;
    }
    case KEY_DOWN: {
      ++mCurrentPauseMenu;
      break;
    }
    case KEY_ENTER: {
      if (mCurrentPauseMenu == ePauseMenu::RESUME) {
        mState = prevState;
        controller.HandleKey(ConvertKey(27));
      } else {
        controller.SetGame(eGame::NO_GAME);
        mState = eState::MENU;
      }
      break;
    }
  }
}
void cliView::KeyPressedGameOverMenu(int key) {
  switch (key) {
    case KEY_UP: {
      --mCurrentGameOverMenu;
      break;
    }
    case KEY_DOWN: {
      ++mCurrentGameOverMenu;
      break;
    }
    case KEY_ENTER: {
      if (mCurrentGameOverMenu == eGameOverMenu::PLAY_AGAIN) {
        mState = prevState;
        if (mState == eState::SNAKE) controller.SetGame(eGame::SNAKE);
        if (mState == eState::TETRIS) controller.SetGame(eGame::TETRIS);
      } else {
        controller.SetGame(eGame::NO_GAME);
        mState = eState::MENU;
      }
      break;
    }
  }
}

void cliView::DrawSnakeGame() {
  // ();
  wclear(main_w);
  controller.GetData(gi);
  for (int i = 0; i < gi.height; i++) {
    for (int j = 0; j < gi.width; j++) {
      if (gi.grid[i][j] == 1) {
        for (int x = j * kMult; x < j * kMult + (1 * kMult); x++) {
          mvwaddch(main_w, i, x, full_block | COLOR_PAIR(6));
        }
      }
      if (gi.grid[i][j] == 2) {
        for (int x = j * kMult; x < j * kMult + (1 * kMult); x++) {
          mvwaddch(main_w, i, x, full_block | COLOR_PAIR(3));
        }
      }
      if (gi.grid[i][j] < 0) {
        for (int x = j * kMult; x < j * kMult + (1 * kMult); x++) {
          mvwaddch(main_w, i, x, full_block | COLOR_PAIR(2));
        }
      }
    }
  }
  refresh();
  wrefresh(main_w);
}
void cliView::DrawTetrisGame() {
  wclear(main_w);
  controller.GetData(gi);
  RenderNextFigurePreview(gi.next_fig);
  RenderPoints();
  RenderHighscore();
  RenderLevel();
  RenderStats();
  RenderStatsValues();
  RenderField();
}
void cliView::DrawPauseMenu() {
  ;
  ;
}
void cliView::DrawGameOverMenu() {
  ;
  ;
}

#include "main_view.h"

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
        for (int x = j * kMult; x < j * MULT + (1 * MULT); x++) {
          mvwaddch(np, i, x,
                   EMPTY_BLOCK | COLOR_PAIR((fig % FIGURES_COUNT) + 1));
        }
      }
      bits >>= 1;
    }
  }
  wrefresh(np);
}

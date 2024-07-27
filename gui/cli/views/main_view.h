#ifndef ASDASDASDASDASDASD
#define ASDASDASDASDASDASD
#include <ncurses.h>

#include "../../../controller/controller.h"

class cliView {
 public:
  cliView();
  //   ~cliView();

  void DrawOverlay();
  void PrintRectangle(int x1, int y1, int x2, int y2);
  void RenderNextFigurePreview(int fig);
  void RenderField();
  void RenderPoints();
  void RenderHighscore();
  void RenderLevel();
  void Processing();
  void InitNcurses();
  // UserAction_t parse_signal();
  enum class eState { MENU, SNAKE, TETRIS, PAUSE, GAMEOVER, EXIT };
  void RenderStats();
  void RenderStatsValues();
  void RenderValues();
  void DrawUsingState();
  void DrawMenu();
  void ApplyKeyUsingState(int key);

  void KeyPressedMenu(int key);
  void KeyPressedTetris(int key);
  void KeyPressedSnake(int key);
  void KeyPressedPauseMenu(int key);
  void KeyPressedGameOverMenu(int key);

  void DrawSnakeGame();
  void DrawTetrisGame();
  void DrawPauseMenu();
  void DrawGameOverMenu();

  bool render_menu_ready = true;
  bool render_pause_menu_ready = true;
  bool render_gameover_menu_ready = true;
  bool tetris_stats_render_ready = true;
  bool overlay_render_ready = true;

  const static int kMult = 2;

  void InitGridAccepter();
  void InitAllMenu();
  void InitNcursesWindows();

  ArcadeGame::eKeys ConvertKey(int aKey);

  GameInfo gi;

  static inline const std::unordered_map<int, ArcadeGame::eKeys> KeyMap = {
      {KEY_UP, ArcadeGame::eKeys::Key_Up},
      {KEY_DOWN, ArcadeGame::eKeys::Key_Down},
      {KEY_LEFT, ArcadeGame::eKeys::Key_Left},
      {KEY_RIGHT, ArcadeGame::eKeys::Key_Right},
      {27, ArcadeGame::eKeys::Key_ESC}};  // esc

  enum class Colors { YELLOW = 8, BLUE, RED, PINK, VIOLET, ORANGE, BROWN };
  static const int tetris_figures_count = 7;
  static inline const std::array<int, tetris_figures_count> preview_masks = {
      0b000100011100, 0b010000011100, 0b011000001100, 0b001100011000,
      0b001000011100, 0b011110000000, 0b001100001100};

  static const char full_block = ' ';

  int tiles_count_h;
  int tiles_count_w;
  eState mState = eState::MENU;
  eState prevState;

  enum class eMenu { TETRIS, SNAKE, EXIT };
  enum class ePauseMenu { RESUME, GO_TO_MAIN_MENU };
  enum class eGameOverMenu { PLAY_AGAIN, GO_TO_MAIN_MENU };

  std::vector<std::pair<eMenu, std::string>> mvMenu;
  std::vector<std::pair<ePauseMenu, std::string>> mvPauseMenu;
  std::vector<std::pair<eGameOverMenu, std::string>> mvGameOverMenu;

  eMenu mCurrentMenu{eMenu::TETRIS};
  ePauseMenu mCurrentPauseMenu{ePauseMenu::RESUME};
  eGameOverMenu mCurrentGameOverMenu{eGameOverMenu::PLAY_AGAIN};

  WINDOW* main_w;
  WINDOW* preview_next_figure_w;
  WINDOW* score_w;
  WINDOW* highscore_w;
  WINDOW* level_w;
  WINDOW* figures_stats_w;

  Controller controller;
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
#endif
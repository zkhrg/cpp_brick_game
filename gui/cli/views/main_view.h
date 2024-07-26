#ifndef ASDASDASDASDASDASD
#define ASDASDASDASDASDASD
#include <ncurses.h>

#include "../../../controller/controller.h"

class cliView {
 public:
  cliView();
  ~cliView();

  void DrawOverlay();
  void PrintRectangle(int x1, int y1, int x2, int y2);
  void render_next_figure_preview(int fig);
  void render_field();
  void render_points();
  void render_highscore();
  void render_level();
  // UserAction_t parse_signal();
  void render_stats();
  void init_game_windows();
  void render_stats_values();
  void render_pause();
  void render_gameover();
  void render_values();

  const static int kMult;

  void InitGridAccepter();

  ArcadeGame::eKeys ConvertKey(int aKey);

  GameInfo gi;

  static inline const std::unordered_map<int, ArcadeGame::eKeys> KeyMap = {
      {KEY_UP, ArcadeGame::eKeys::Key_Up},
      {KEY_DOWN, ArcadeGame::eKeys::Key_Down},
      {KEY_LEFT, ArcadeGame::eKeys::Key_Left},
      {KEY_RIGHT, ArcadeGame::eKeys::Key_Right},
      {27, ArcadeGame::eKeys::Key_ESC}};  // esc

  enum class Colors { YELLOW = 8, BLUE, RED, PINK, VIOLET, ORANGE, BROWN };

  static inline const std::array<int, 7> preview_masks = {
      0b000100011100, 0b010000011100, 0b011000001100, 0b001100011000,
      0b001000011100, 0b011110000000, 0b001100001100};

  static const char full_block = ' ';

  int tiles_count_h;
  int tiles_count_w;

  WINDOW* main_w;
  WINDOW* preview_next_figure_w;
  WINDOW* score_w;
  WINDOW* highscore_w;
  WINDOW* level_w;
  WINDOW* figures_stats_w;
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

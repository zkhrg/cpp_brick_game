#include "backend_tetris.h"

namespace s21 {

TetrisGame::TetrisGame() : game_last_tick_time(0), level(0), gen_(rd_()) {
  cur_figure = (eFigure)(distrib_(gen_) % figures_count);
  next_figure = (eFigure)(distrib_(gen_) % figures_count);
  InitTetramino();
  InitField(20, 10);
  AddTetraminoToField();
  game_highscore = 0;
  ReadHighscore();
  game_speed = 0;
  game_tmp_speed = 0;
  game_state = eGameState::START;
  render_ready = true;
  stats_render_ready = true;
  values_render_ready = true;

  for (int i = 0; i < figures_count; i++) figures_stats[i] = 0;
  figures_stats[(int)cur_figure]++;
}

void TetrisGame::InitTetramino() {
  for (int i = 0; i < 8; i++) {
    tetramino_coords[i / 2][i % 2] =
        (int)(FIGURES_COORDS[(int)cur_figure][i] - '0');
  }
  tetramino_corner[0] = -1;
  tetramino_corner[1] = (int)(FIGURES_COORDS[(int)cur_figure][8] - '0');
}

void TetrisGame::ClearPos() {
  for (int i = 0; i < 4; i++) {
    field_grid[tetramino_coords[i][0] * field_width + tetramino_coords[i][1]] =
        empty_tile;
  }
}

void TetrisGame::AddTetraminoToField() {
  for (int i = 0; i < 4; i++) {
    field_grid[tetramino_coords[i][0] * field_width + tetramino_coords[i][1]] =
        (int)cur_figure + tetramino_tile;
  }
}

void TetrisGame::ApplyFall() {
  for (int i = 0; i < 4; i++) {
    field_grid[tetramino_coords[i][0] * field_width + tetramino_coords[i][1]] +=
        figures_count;
  }
}

void TetrisGame::InitField(int h, int w) {
  field_height = h;
  field_width = w;
  field_grid = new int[h * w]();
}

void TetrisGame::MakeFallTetramino() {
  ClearPos();

  for (int i = 0; i < 4; i++) {
    tetramino_coords[i][0]++;
  }
  tetramino_corner[0]++;

  AddTetraminoToField();
}

bool TetrisGame::IsTetraminoFell() {
  bool res = false;
  for (int i = 0; i < 4; i++) {
    if ((tetramino_coords[i][0] == (field_height - 1)) ||
        (field_grid[(tetramino_coords[i][0] + 1) * field_width +
                    tetramino_coords[i][1]] > figures_count)) {
      res = true;
    }
  }
  return res;
}

void TetrisGame::MoveTetramino(eDirection d) {
  if (!CanTetraminoMove(d)) return;

  ClearPos();
  for (int i = 0; i < 4; i++) {
    tetramino_coords[i][1] += (int)d;
  }
  tetramino_corner[1] += (int)d;

  AddTetraminoToField();
  render_ready = true;
}

bool TetrisGame::CanTetraminoMove(eDirection d) {
  bool res = (game_state == eGameState::START) ? true : false;
  int wall = (d == eDirection::LEFT) ? 0 : field_width - 1;
  for (int i = 0; i < 4; i++) {
    res *= (tetramino_coords[i][1] != wall &&
            field_grid[tetramino_coords[i][0] * field_width +
                       tetramino_coords[i][1] + (int)d] <
                figures_count + tetramino_tile);
  }
  return res;
}

void TetrisGame::FieldContentOneLineDown(int dest) {
  for (int i = dest; i >= 1; i--) {
    for (int j = 0; j < field_width; j++) {
      int tmp = field_grid[(i - 1) * field_width + j];
      field_grid[(i - 1) * field_width + j] = field_grid[i * field_width + j];
      field_grid[i * field_width + j] = tmp;
    }
  }
}

void TetrisGame::ClearGlassLine(int line) {
  for (int j = 0; j < field_width; j++) {
    field_grid[line * field_width + j] = empty_tile;
  }
}

int TetrisGame::CountFilledLines() {
  int counter = 0;
  int cleared_lines[4] = {0};
  for (int i = field_height, complete_line = 1; i >= 0;
       i--, complete_line = 1) {
    for (int j = 0; j < field_width; j++) {
      complete_line *= (field_grid[i * field_width + j] > figures_count);
    }
    if (complete_line) {
      ClearGlassLine(i);
      cleared_lines[counter] = i;
      counter++;
    }
  }
  for (int i = counter - 1; i >= 0; i--) {
    FieldContentOneLineDown(cleared_lines[i]);
  }
  return counter;
}

void TetrisGame::AddPoints() {
  switch (CountFilledLines()) {
    case 1:
      game_points += 100;
      break;
    case 2:
      game_points += 300;
      break;
    case 3:
      game_points += 700;
      break;
    case 4:
      game_points += 1500;
      break;
  }
  if (game_points > game_highscore) {
    game_highscore = game_points;
    WriteHighscore();
  }
  SetNewSpeed();
}

void TetrisGame::SetNewSpeed() {
  if ((game_points / 600) >= levels_count) {
    game_tmp_speed = levels_count - 1;
  } else {
    game_tmp_speed = (game_points / 600);
  }
}

int TetrisGame::GetFigureBoxSize(eFigure fig) {
  return (fig == eFigure::I || fig == eFigure::O) ? 4 : 3;
}

bool TetrisGame::CanFigureRotate(eFigure fig) {
  bool res = 1;
  int size = GetFigureBoxSize(fig);
  if (tetramino_corner[0] < field_height - (size - 1) &&
      tetramino_corner[1] < field_width - (size - 1) &&
      tetramino_corner[0] >= 0 && tetramino_corner[1] >= 0) {
    for (int i = tetramino_corner[0]; i < tetramino_corner[0] + size; i++) {
      for (int j = tetramino_corner[1]; j < tetramino_corner[1] + size; j++) {
        res *=
            (field_grid[i * field_width + j] < figures_count + tetramino_tile);
      }
    }
  } else {
    res = 0;
  }
  return res;
}

void TetrisGame::RotateFigure() {
  if (!CanFigureRotate(cur_figure)) return;

  int size = GetFigureBoxSize(cur_figure);
  int* m = new int[size * size]();
  int* m1 = new int[size * size]();
  for (int i = tetramino_corner[0]; m && i < tetramino_corner[0] + size; i++) {
    for (int j = tetramino_corner[1]; j < tetramino_corner[1] + size; j++) {
      m[((i - tetramino_corner[0]) * size) + (j - tetramino_corner[1])] =
          field_grid[i * field_width + j];
    }
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      m1[j * size + i] = m[(size - i - 1) * size + j];
    }
  }

  ClearPos();

  for (int i = 0, c = 0; i < size && c < 4; i++) {
    for (int j = 0; j < size && c < 4; j++) {
      if (m1[i * size + j] == (int)cur_figure + tetramino_tile) {
        tetramino_coords[c][0] = tetramino_corner[0] + i;
        tetramino_coords[c][1] = tetramino_corner[1] + j;
        c++;
      }
    }
  }
  AddTetraminoToField();
  if (m) delete[] m;
  if (m1) delete[] m1;
  render_ready = true;
}

void TetrisGame::TogglePause() {
  render_ready = true;
  game_state =
      (game_state == eGameState::PAUSE) ? eGameState::START : eGameState::PAUSE;
}

// apply signal to state
void TetrisGame::ApplySignal() {
  // if (sig == Pause) toggle_pause(tp);
  // if (sig == Terminate) tp->game_state = final_state;
  // if (sig == Left) move_tetramino(tp, left_direction);
  // if (sig == Right) move_tetramino(tp, right_direction);
  // if (sig == Up) rotate_figure(tp);
  // if (sig == Down) tp->game_last_tick_time = 0;
}

bool TetrisGame::IsTimeToTick() {
  bool res = false;
  long long cur_time = GetTimeMs();
  if (cur_time - game_last_tick_time > times[game_speed]) {
    res = (game_state == eGameState::START) ? true : false;
    game_last_tick_time = cur_time;
  }
  return res;
}

void TetrisGame::MakeTick() {
  if (!IsTimeToTick()) return;

  if (IsTetraminoFell()) {
    ApplyFall();
    int tmp_points = game_points;
    AddPoints();
    if (tmp_points - game_points != 0) values_render_ready = true;
    cur_figure = next_figure;
    InitTetramino();
    if (CanAddTetramino()) {
      figures_stats[(int)cur_figure]++;
      next_figure = (eFigure)(distrib_(gen_) % figures_count);
      AddTetraminoToField();
      stats_render_ready = true;
    } else {
      game_state = eGameState::GAMOVER;
    }
  } else {
    MakeFallTetramino();
    game_speed = game_tmp_speed;
  }
  render_ready = true;
}

long long TetrisGame::GetTimeMs() {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void TetrisGame::ReadHighscore() {
  FILE* f = fopen("highscore", "rb");
  if (f) {
    fread(&(game_highscore), sizeof(int), 1, f);
    fclose(f);
  }
}

void TetrisGame::WriteHighscore() {
  FILE* f = fopen("highscore", "wb");
  if (f) {
    fwrite(&(game_highscore), sizeof(int), 1, f);
    fclose(f);
  }
}

bool TetrisGame::CanAddTetramino() {
  bool res = true;
  for (int i = 0; i < 4; i++) {
    res *= field_grid[tetramino_coords[i][0] * field_width +
                      tetramino_coords[i][1]] == empty_tile;
  }
  return res;
}

GameInfo TetrisGame::GetData() const {
  GameInfo gi{};
  gi.grid = new int*[field_height];
  for (int i = 0; i < field_height; ++i) {
    gi.grid[i] = new int[field_width];
    for (int j = 0; j < field_width; ++j) {
      gi.grid[i][j] = field_grid[i * field_width + j];
    }
  }
  gi.height = field_height;
  gi.width = field_width;
  gi.state = (eCommonTypesState)game_state;
  gi.next_fig = (int)next_figure;
  return gi;
}

void TetrisGame::HandleKey(eKeys k) {
  switch (k) {
    case eKeys::Key_Up: {
      RotateFigure();
      break;
    }
    case eKeys::Key_Down: {
      game_last_tick_time = 0;
      break;
    }
    case eKeys::Key_Left: {
      MoveTetramino(eDirection::LEFT);
      break;
    }
    case eKeys::Key_Right: {
      MoveTetramino(eDirection::RIGHT);
      break;
    }
    case eKeys::Key_ESC: {
      TogglePause();
      break;
    }
    default: {
      break;
    }
  }
}
};  // namespace s21

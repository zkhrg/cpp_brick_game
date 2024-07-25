#include "backend_snake.h"

namespace s21 {
SnakeGame::SnakeGame()
    : height_(20),
      width_(10),
      level(0),
      last_tick_time(0),
      dir(Direction::Up),
      gen_(rd_()),
      distrib_(0, 20 * 10 - 1) {
  ;
  state = eState::GAMING;
  ready_change_dir = true;
  snake_body.push_back({9, 4});
  CreateField();
  AddSnakeToField();
  GenerateAppleIndex();
  AddFoodToField();
}
SnakeGame::SnakeGame(int h, int w) {}
SnakeGame::~SnakeGame() { FreeField(); }

void SnakeGame::CreateField() {
  field_grid_ = new int*[height_];
  for (int i = 0; i < height_; i++) {
    field_grid_[i] = new int[width_];
    for (int j = 0; j < width_; j++) {
      field_grid_[i][j] = 0;
    }
  }
}

void SnakeGame::PrintField() {
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      if (!field_grid_[i][j])
        printf("%c", '_');
      else if (field_grid_[i][j] == 1) {
        printf("%c", 'X');
      } else {
        printf("%c", '@');
      }
    }
    std::cout << std::endl;
  }
}

void SnakeGame::FreeField() {
  for (int i = 0; i < height_; i++) {
    delete[] field_grid_[i];
  }
  delete[] field_grid_;
}

int SnakeGame::FindSpaceToSpawnApple(int ind) {
  int res = -1;
  int start_i = ind / width_;
  int start_j = ind % width_;
  for (int i = start_i; res == -1 && i < height_; i++) {
    for (int j = start_j; res == -1 && j < width_; j++) {
      if (field_grid_[i][j] == 0) res = i * width_ + j;
    }
  }
  for (int i = 0; res == -1 && i <= start_i; i++) {
    for (int j = 0; res == -1 && j < width_; j++) {
      if (field_grid_[i][j] == 0) res = i * width_ + j;
    }
  }
  return res;
}

void SnakeGame::GenerateAppleIndex() {
  int v = FindSpaceToSpawnApple((int)distrib_(gen_));
  if (v != -1) {
    food_ind = {v / get_width(), v % get_width()};
  }
  // можно зарейзить исключение и потом его поймать
}

int SnakeGame::get_height() const { return height_; }
int SnakeGame::get_width() const { return width_; }

bool SnakeGame::isTimeToTick() {
  bool res = false;
  long long cur_time = GetTimeMs();
  if (cur_time - last_tick_time > times[level]) {
    res = (state == eState::GAMING) ? true : false;
    last_tick_time = cur_time;
    ready_change_dir = true;
  }
  return res;
}

long long SnakeGame::GetTimeMs() {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000);
}

void SnakeGame::MakeTick() {
  if (!isTimeToTick()) return;
  if (CheckStopCollision()) {
    SetState(eState::GAMEOVER);
  } else if (CheckFoodCollision()) {
    GrowSnake();
    GenerateAppleIndex();
  } else {
    MoveSnake();
  }
  ClearField();
  AddFoodToField();
  AddSnakeToField();
}

void SnakeGame::ClearField() {
  for (int i = 0; i < get_height(); i++) {
    for (int j = 0; j < get_width(); j++) {
      field_grid_[i][j] = 0;
    }
  }
}

void SnakeGame::AddSnakeToField() {
  for (const auto& elem : snake_body) {
    field_grid_[elem.first][elem.second] = 1;
  }
  int head = -1;
  switch (dir) {
    case Direction::Up: {
      head = -1;
      break;
    }
    case Direction::Down: {
      head = -2;
      break;
    }
    case Direction::Left: {
      head = -3;
      break;
    }
    case Direction::Right: {
      head = -4;
      break;
    }
  }
  field_grid_[(*snake_body.begin()).first][(*snake_body.begin()).second] = head;
}

void SnakeGame::AddFoodToField() {
  field_grid_[food_ind.first][food_ind.second] = 2;
}

/**
 * @brief Движение головы змейки по направлению из класса.
 *
 */
void SnakeGame::MoveSnake() {
  auto it = snake_body.begin();
  std::pair<int, int> new_head = *it;
  new_head.first += m[dir].first;
  new_head.second += m[dir].second;
  snake_body.push_front(new_head);
  snake_body.pop_back();
}
/**
 * @brief Рост змейки с движением головы но не удалением хвоста.
 *
 */
void SnakeGame::GrowSnake() {
  auto it = snake_body.begin();
  std::pair<int, int> new_head = *it;
  new_head.first += m[dir].first;
  new_head.second += m[dir].second;
  snake_body.push_front(new_head);
}
/**
 * @brief Устанавливает направление движения змейки c проверкой условия что это
 * не смена движения на 180 градусов
 *
 * @param val
 */
void SnakeGame::set_direction(Direction val) {
  if (!ready_change_dir) return;
  if ((dir == Direction::Down && val == Direction::Up) ||
      (dir == Direction::Up && val == Direction::Down) ||
      (dir == Direction::Left && val == Direction::Right) ||
      (dir == Direction::Right && val == Direction::Left)) {
    return;
  }
  ready_change_dir = false;
  dir = val;
}

/**
 * @brief Проверка столкновения с едой
 *
 * @return true столкнулись с едой
 * @return false не столкнулись с едой
 */
bool SnakeGame::CheckFoodCollision() { return food_ind == *snake_body.begin(); }

/**
 * @brief Проверка столкновения которое ведет к завершению игры
 *
 * @return true столкнулись с сегментом / стенкой.
 * @return false не столкнулись с сегментом / стенкой.
 */
bool SnakeGame::CheckStopCollision() {
  auto head = std::pair<int, int>{(*snake_body.begin()).first + m[dir].first,
                                  (*snake_body.begin()).second + m[dir].second};

  bool res = head.first < 0 || head.second < 0 || head.first >= get_height() ||
             head.second >= get_width();

  if (res != 1) {
    auto next_tile = field_grid_[head.first][head.second];
    res = (next_tile != 0 && next_tile != 2);
  }

  return res;
}

// для отдачи данных мне делаем их копию в кучу чтобы СЛУЧАЙНО не изменить
// TODO: need to free
GameInfo SnakeGame::GetData() const {
  GameInfo gi{};
  gi.grid = new int*[get_height()];
  for (int i = 0; i < get_height(); ++i) {
    gi.grid[i] = new int[get_width()];
    for (int j = 0; j < get_width(); ++j) {
      gi.grid[i][j] = field_grid_[i][j];
    }
  }
  gi.height = get_height();
  gi.width = get_width();
  gi.state = (eCommonTypesState)state;
  gi.points = snake_body.size() - 1;
  return gi;
}

void SnakeGame::HandleKey(eKeys k) {
  switch (k) {
    case eKeys::Key_Up: {
      set_direction(Direction::Up);
      break;
    }
    case eKeys::Key_Down: {
      set_direction(Direction::Down);
      break;
    }
    case eKeys::Key_Left: {
      set_direction(Direction::Left);
      break;
    }
    case eKeys::Key_Right: {
      set_direction(Direction::Right);
      break;
    }
    case eKeys::Key_ESC: {
      SetState(eState::PAUSE);
      break;
    }
    default: {
      break;
    }
  }
}

void SnakeGame::SetState(eState s) {
  if (state == eState::PAUSE && s == eState::PAUSE) {
    state = eState::GAMING;
  } else if (state == eState::GAMING && s == eState::PAUSE) {
    state = eState::PAUSE;
  } else if (s == eState::GAMEOVER) {
    state = s;
  }
}
SnakeGame::eState SnakeGame::GetState() { return state; }
}  // namespace s21

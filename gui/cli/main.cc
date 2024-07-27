// #include <locale.h>
#include <string.h>

// #include <algorithm>
// #include <iostream>
// #include <locale>

#include "views/main_view.h"

int main(void) {
  setlocale(LC_ALL, "");
  cliView v;
  while (v.mState != cliView::eState::EXIT) {
    v.ApplyKeyUsingState(getch());
    v.Processing();
    v.DrawUsingState();
  }
  endwin();
  return 0;
}
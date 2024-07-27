// #include <locale.h>
#include <string.h>

// #include <algorithm>
// #include <iostream>
// #include <locale>

#include "views/main_view.h"

void start_game() {
  //

  // while (tp->game_state != final_state) {
  //   apply_signal(tp, parse_signal());
  //   if (tp->render_ready) render_field(tp, gw->main);
  //   if (tp->stats_render_ready) {
  //     render_stats_values(tp, gw->figures_stats);
  //     render_next_figure_preview(gw->preview_next_figure, tp->next_figure);
  //   }
  //   if (tp->values_render_ready) render_values(tp, gw);
  //   tick_logic(tp);
  // }
  // free(tp->field_grid);
}

int main(void) {
  setlocale(LC_ALL, "");
  cliView v;
  int k;
  while ((k = getch()) != 'q') {
    v.DrawUsingState();
    v.ApplyKeyUsingState(k);
  }
  getchar();
  endwin();
  return 0;
}
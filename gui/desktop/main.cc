#include <QApplication>

#include "views/main_view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  glView w;
  w.show();

  return a.exec();
}

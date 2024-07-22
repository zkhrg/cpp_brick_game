#include <QApplication>
#include <iostream>

#include "glview.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  glView w;
  w.show();
  std::cout << "asd" << std::endl;

  return a.exec();
}

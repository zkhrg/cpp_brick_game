#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <QFontDatabase>
#include <QMap>
#include <QString>

class FontManager {
 public:
  FontManager();
  ~FontManager();

  bool loadFont(const QString& name, const QString& path);
  QFont getFont(const QString& name);

 private:
  QMap<QString, int> fonts;
};

#endif  // FONT_MANAGER_H

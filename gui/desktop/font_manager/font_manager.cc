#include "font_manager.h"

#include <QDebug>

FontManager::FontManager() {}

FontManager::~FontManager() {}

bool FontManager::loadFont(const QString& name, const QString& path) {
  int fontId = QFontDatabase::addApplicationFont(path);
  if (fontId == -1) {
    qWarning() << "Failed to load font:" << path;
    return false;
  }

  fonts[name] = fontId;
  return true;
}

QFont FontManager::getFont(const QString& name) {
  if (fonts.contains(name)) {
    QStringList fontFamilies =
        QFontDatabase::applicationFontFamilies(fonts[name]);
    if (!fontFamilies.isEmpty()) {
      return QFont(fontFamilies.at(0));
    }
  }
  qWarning() << "Font not found:" << name;
  return QFont();
}

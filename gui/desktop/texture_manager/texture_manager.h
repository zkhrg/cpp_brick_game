#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <QMap>
#include <QOpenGLTexture>
#include <QString>

class TextureManager {
 public:
  TextureManager();
  ~TextureManager();

  bool loadTexture(const QString& name, const QString& path);
  QOpenGLTexture* getTexture(const QString& name);

 private:
  QMap<QString, QOpenGLTexture*> textures;
};

#endif  // TEXTURE_MANAGER_H

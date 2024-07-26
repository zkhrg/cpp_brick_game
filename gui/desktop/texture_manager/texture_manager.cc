#include "texture_manager.h"

#include <QImage>

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
  qDeleteAll(textures);
  textures.clear();
}

bool TextureManager::loadTexture(const QString& name, const QString& path) {
  QImage img(path);
  if (img.isNull()) {
    qWarning("Failed to load texture: %s", qPrintable(path));
    return false;
  }

  QOpenGLTexture* texture = new QOpenGLTexture(img.mirrored());
  texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
  texture->setMagnificationFilter(QOpenGLTexture::Linear);
  texture->setWrapMode(QOpenGLTexture::Repeat);

  textures[name] = texture;
  return true;
}

QOpenGLTexture* TextureManager::getTexture(const QString& name) {
  return textures.value(name, nullptr);
}

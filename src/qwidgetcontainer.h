#pragma once

#include <QOpenGLTexture>

#include <QVector3D>

#include <QObject>

class QWidget;

namespace qpointsview {

class QWidgetContainer final : public QObject
{
public:
  QWidgetContainer(QObject* parent, QWidget* widget)
    : QObject(parent)
    , m_widget(widget)
  {}

  int width() const;

  int height() const;

  void setPosition(const QVector3D& p) { m_position = p; }

  void setNormal(const QVector3D& n) { m_normal = n; }

  bool paintOntoTexture();

  QOpenGLTexture& texture() { return m_texture; }

private:
  QWidget* m_widget;

  QVector3D m_position{ 0, 0, 0 };

  QVector3D m_normal{ 0, 0, -1 };

  QOpenGLTexture m_texture{ QOpenGLTexture::Target2D };
};

} // namespace qpointsview

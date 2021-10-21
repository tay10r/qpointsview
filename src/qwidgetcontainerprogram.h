#pragma once

#include <QOpenGLShaderProgram>

#include <QOpenGLBuffer>

class QMatrix4x4;

namespace qpointsview {

class QWidgetContainer;

class QWidgetContainerProgram final
{
public:
  bool init();

  void render(QWidgetContainer* container, const QMatrix4x4& mvp);

  float metersPerPixel() const noexcept { return m_metersPerPixel; }

  void setMetersPerPixel(float metersPerPixel)
  {
    m_metersPerPixel = metersPerPixel;
  }

private:
  QOpenGLShaderProgram m_program;

  QOpenGLBuffer m_vertexBuffer;

  float m_metersPerPixel = 0.01;

  int m_positionLocation = -1;

  int m_widthLocation = -1;

  int m_heightLocation = -1;

  int m_mvpLocation = -1;
};

} // namespace qpointsview

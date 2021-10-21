#include "qwidgetcontainerprogram.h"

#include "qwidgetcontainer.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

namespace qpointsview {

bool
QWidgetContainerProgram::init()
{
  if (!m_vertexBuffer.create())
    return false;

  m_vertexBuffer.bind();

  // clang-format off
  float vertices[12]{
    0, 0,
    0, 1,
    1, 0,
    1, 1
  };
  // clang-format on

  m_vertexBuffer.allocate(vertices, sizeof(vertices));

  m_vertexBuffer.release();

  m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                    ":/render_widget.vert");

  m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                    ":/render_widget.frag");

  m_program.link();

  assert(m_program.isLinked());

  [[maybe_unused]] bool success = m_program.bind();

  assert(success);

  m_positionLocation = m_program.attributeLocation("position");

  assert(m_positionLocation >= 0);

  m_mvpLocation = m_program.uniformLocation("mvp");

  assert(m_mvpLocation >= 0);

  m_widthLocation = m_program.uniformLocation("width");

  assert(m_widthLocation >= 0);

  m_heightLocation = m_program.uniformLocation("height");

  assert(m_heightLocation >= 0);

  m_program.release();

  return true;
}

void
QWidgetContainerProgram::render(QWidgetContainer* container,
                                const QMatrix4x4& mvp)
{
  const float width = container->width() * m_metersPerPixel;

  const float height = container->height() * m_metersPerPixel;

  m_program.bind();

  m_program.setUniformValue(m_mvpLocation, mvp);

  m_program.setUniformValue(m_widthLocation, width);

  m_program.setUniformValue(m_heightLocation, height);

  m_vertexBuffer.bind();

  container->texture().bind();

  QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();

  functions->glEnableVertexAttribArray(m_positionLocation);

  functions->glVertexAttribPointer(
    m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

  functions->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  functions->glDisableVertexAttribArray(m_positionLocation);

  container->texture().release();

  m_vertexBuffer.release();

  m_program.release();
}

} // namespace qpointsview

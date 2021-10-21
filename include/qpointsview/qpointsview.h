#pragma once

#ifndef QPOINTSVIEW_QPOINTSVIEW_H
#define QPOINTSVIEW_QPOINTSVIEW_H

#include <QOpenGLWidget>

template<typename T>
class QVector;

class QMatrix4x4;
class QMouseEvent;
class QKeyEvent;

namespace qpointsview {

class QPointColorPair;
class QAbstractCamera;
class QPointsViewPrivate;

class QPointsView : public QOpenGLWidget
{
  Q_OBJECT
public:
  QPointsView(QWidget* parent);

  ~QPointsView();

  /// Assigns the camera to receive mouse and keyboard events from the points
  /// view.
  ///
  /// @param camera The camera to assign the view. This widget does not take
  ///               ownership of the pointer. The camera must remain alive until
  ///               it is either removed from the widget or until the widget is
  ///               destroyed.
  void setCamera(QAbstractCamera* camera);

  void setPointRadius(float radius);

  void setViewMatrix(const QMatrix4x4& viewMatrix);

  void setProjectionMatrix(const QMatrix4x4& projectionMatrix);

  /// @note Only after the OpenGL context has been initialized will this
  ///       function return a valid value.
  float getPointRadiusMin() const;

  /// @note Only after the OpenGL context has been initialized will this
  ///       function return a valid value.
  float getPointRadiusMax() const;

  /// Uploads points into the vertex buffer to be rendered.
  /// This function must only be called after the OpenGL context is initialized.
  ///
  /// @return True on success, false if the OpenGL context has not yet been
  ///         initialized.
  bool setPoints(const QVector<QPointColorPair>& pointColorPairs);

signals:
  void contextInitialized();

  void resized(int w, int h);

private:
  friend QPointsViewPrivate;

  void initializeGL() override;

  void paintGL() override;

  void resizeGL(int, int) override;

  void mousePressEvent(QMouseEvent*) override;

  void mouseReleaseEvent(QMouseEvent*) override;

  void mouseMoveEvent(QMouseEvent*) override;

  void keyPressEvent(QKeyEvent*) override;

  void keyReleaseEvent(QKeyEvent*) override;

  void updateInput();

private:
  QPointsViewPrivate* m_self;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QPOINTSVIEW_H

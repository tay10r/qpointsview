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

  /// Adds a widget to the 3D scene. This can be used to label areas in the
  /// scene or show additional sensor data.
  ///
  /// @param widget The widget to add to the scene.
  ///
  /// @return A pointer to the widget that was added.
  QWidget* addWidget(QWidget* widget);

  /// Removes a widget from the scene.
  ///
  /// @param widget The widget to remove.
  void removeWidget(QWidget* widget);

  /// Sets the world space position of a widget.
  /// The position of a widget is considered the top left corner of the widget.
  ///
  /// @param widget The widget to set the position of.
  ///
  /// @param p The position in world space to assign the widget.
  void setWidgetPosition(QWidget* widget, const QVector3D& p);

  /// Sets the normal of the plane that the widget resides in.
  ///
  /// @param widget The widget to set the normal of.
  ///
  /// @param n The normal of the widget. This indicates which way the widget is
  ///          facing.
  void setWidgetNormal(QWidget* widget, const QVector3D& n);

  /// Sets the number of meters per pixel, when it comes to widgets.
  /// This directly affects the apparent image quality of a widget. The lower
  /// this value is, the higher resolution the widget appears to be in.
  ///
  /// @note The default is one centimeter per pixel.
  ///
  /// @param metersPerPixel The meters per pixel to render the widgets as.
  void setMetersPerWidgetPixel(float metersPerPixel);

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

#pragma once

#ifndef QPOINTSVIEW_QPOINTSVIEW_H
#define QPOINTSVIEW_QPOINTSVIEW_H

#include <QFrame>

template<typename T>
class QVector;

class QMatrix4x4;
class QVector3D;
class QColor;

namespace qpointsview {

class QPointsViewPrivate;

class QPointsView : public QFrame
{
public:
  QPointsView(QWidget* parent);

  ~QPointsView();

  void setPointRadius(float radiusNear, float radiusFar = 0.0f);

  void setPoints(QVector<QVector3D>&& points);

  void setColors(QVector<QColor>&& colors);

  void setViewMatrix(const QMatrix4x4& viewMatrix);

  void setProjectionMatrix(const QMatrix4x4& projectionMatrix);

  /// This function is used to convert a set of points in world space to screen
  /// space. This does not need to be called by the end-user. It is exposed only
  /// for testing purposes.
  ///
  /// @param mvp A combination of the model, view, and projection matrices.
  ///
  /// @param width The width of the screen space, in pixels.
  ///
  /// @param height The height of the screen space, in pixels.
  ///
  /// @param in The points to be converted.
  ///
  /// @param pointCount The number of points to convert.
  ///
  /// @param out A pointer to a buffer of at least @p pointCount size that can
  ///            fit the converted points.
  ///
  /// @return The number of points that ended up in screen space.
  static int toScreenSpace(const QMatrix4x4& mvp,
                           int width,
                           int height,
                           const QVector3D* in,
                           int pointCount,
                           QVector3D* out);

private:
  QPointsViewPrivate* m_self;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QPOINTSVIEW_H

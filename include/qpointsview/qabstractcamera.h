#pragma once

#ifndef QPOINTSVIEW_QABSTRACTCAMERA_H
#define QPOINTSVIEW_QABSTRACTCAMERA_H

#include <QObject>

class QMouseEvent;
class QKeyEvent;

namespace qpointsview {

/// This is the base class of a camera. A camera object is responsible for
/// controlling the view matrix through user input.
class QAbstractCamera : public QObject
{
public:
  QAbstractCamera(QObject* parent);

  virtual ~QAbstractCamera() = default;

  virtual QMatrix4x4 viewMatrix() const = 0;

  virtual void keyPressEvent(QKeyEvent*) = 0;

  virtual void keyReleaseEvent(QKeyEvent*) = 0;

  virtual void mouseMoveEvent(QMouseEvent*) = 0;

  virtual void mousePressEvent(QMouseEvent*) = 0;

  virtual void mouseReleaseEvent(QMouseEvent*) = 0;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QABSTRACTCAMERA_H

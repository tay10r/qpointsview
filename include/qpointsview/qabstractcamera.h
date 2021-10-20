#pragma once

#ifndef QPOINTSVIEW_QABSTRACTCAMERA_H
#define QPOINTSVIEW_QABSTRACTCAMERA_H

#include <QObject>

namespace qpointsview {

/// This is the base class of a camera. A camera object is responsible for
/// controlling the view matrix through user input.
class QAbstractCamera : public QObject
{
public:
  QAbstractCamera(QObject* parent);

  virtual ~QAbstractCamera();

  virtual QMatrix4x4 viewMatrix() const = 0;

  virtual void dragEvent(float dx, float dy) = 0;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QABSTRACTCAMERA_H

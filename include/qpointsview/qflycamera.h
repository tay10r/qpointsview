#pragma once

#ifndef QPOINTSVIEW_QFLYCAMERA_H
#define QPOINTSVIEW_QFLYCAMERA_H

#include <qpointsview/QAbstractCamera>

#include <QVector3D>

namespace qpointsview {

class QFlyCamera final : public QAbstractCamera
{
public:
  QFlyCamera(QObject* parent);

  void dragEvent(float dx, float dy) override;

  QMatrix4x4 viewMatrix() const override;

  void setPosition(const QVector3D& pos) { m_position = pos; }

  /// @param pitch The angle around the perpendicular horizontal axis, in
  /// degrees.
  void setPitch(float pitch);

  /// @param yaw The angle around the vertical axis, in degrees.
  void setYaw(float yaw);

  void setUp(const QVector3D& up) { m_up = up; }

  QVector3D direction() const;

private:
  QVector3D m_position{ 0, 0, 0 };

  QVector3D m_up{ 0, 1, 0 };

  float m_pitch = 0;

  float m_yaw = 0;

  float m_angularSpeed = 2;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QFLYCAMERA_H

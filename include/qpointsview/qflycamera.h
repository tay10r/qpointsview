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

  void moveEvent(float dx, float dy) override;

  QMatrix4x4 viewMatrix() const override;

  void setPosition(const QVector3D& pos) { m_position = pos; }

  void setPitch(float pitch) { m_pitch = pitch; }

  void setYaw(float yaw) { m_yaw = yaw; }

  void setUp(const QVector3D& up) { m_up = up; }

  void setMoveSpeed(float speed) { m_moveSpeed = speed; }

  void setDragSpeed(float speed) { setDragSpeed(speed, speed); }

  void setDragSpeed(float hSpeed, float vSpeed);

  QVector3D up() const noexcept { return m_up; }

  QVector3D direction() const;

  QVector3D right() const { return QVector3D::crossProduct(direction(), up()); }

private:
  QVector3D m_position{ 0, 0, 0 };

  QVector3D m_up{ 0, 1, 0 };

  float m_pitch = 0;

  float m_yaw = 0;

  float m_hDragSpeed = 2;

  float m_vDragSpeed = 2;

  float m_moveSpeed = 1;
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QFLYCAMERA_H

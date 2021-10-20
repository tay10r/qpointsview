#include <qpointsview/QFlyCamera>

#include <QMatrix4x4>

#include <QtMath>

#include <algorithm>

namespace qpointsview {

QFlyCamera::QFlyCamera(QObject* parent)
  : QAbstractCamera(parent)
{}

QMatrix4x4
QFlyCamera::viewMatrix() const
{
  QMatrix4x4 out;

  out.lookAt(m_position, m_position + direction(), m_up);

  return out;
}

QVector3D
QFlyCamera::direction() const
{
  const float x = std::cos(m_yaw) * std::sin(m_pitch);
  const float y = std::sin(m_yaw) * std::sin(m_pitch);
  const float z = std::cos(m_pitch);
  return QVector3D(y, z, -x);
}

void
QFlyCamera::dragEvent(float dx, float dy)
{
  const float epsilon = 0.00001;

  m_pitch += dy * M_PI * m_vDragSpeed;

  m_yaw += dx * 2 * M_PI * m_hDragSpeed;

  m_pitch = std::max(std::min(m_pitch, float(M_PI) - epsilon), epsilon);
}

void
QFlyCamera::moveEvent(float dx, float dy)
{
  m_position += direction() * (dy * m_moveSpeed);

  m_position += right() * (dx * m_moveSpeed);
}

void
QFlyCamera::setDragSpeed(float h, float v)
{
  m_hDragSpeed = h;
  m_vDragSpeed = v;
}

} // namespace qpointsview

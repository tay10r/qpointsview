#pragma once

#ifndef QPOINTSVIEW_QPOINTCOLORPAIR_H
#define QPOINTSVIEW_QPOINTCOLORPAIR_H

#include <QVector3D>

#include <algorithm>

namespace qpointsview {

class QPointColorPair final
{
public:
  QPointColorPair() = default;

  QPointColorPair(const QVector3D& pos, const QColor& color)
  {
    setPosition(pos);
    setColor(color);
  }

  void setColor(const QColor& color)
  {
    setColori(color.red(), color.green(), color.blue(), color.alpha());
  }

  void setColorf(float r, float g, float b, float a = 1.0f)
  {
    rgba[0] = clamp(r * 255, 0.0f, 255.0f);
    rgba[1] = clamp(g * 255, 0.0f, 255.0f);
    rgba[2] = clamp(b * 255, 0.0f, 255.0f);
    rgba[3] = clamp(a * 255, 0.0f, 255.0f);
  }

  void setColori(unsigned char r,
                 unsigned char g,
                 unsigned char b,
                 unsigned char a = 255)
  {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
  }

  void setPosition(float x, float y, float z)
  {
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
  }

  void setPosition(const QVector3D& p)
  {
    xyz[0] = p.x();
    xyz[1] = p.y();
    xyz[2] = p.z();
  }

private:
  template<typename Scalar>
  static float clamp(Scalar x, Scalar min, Scalar max)
  {
    return std::min(std::max(x, min), max);
  }

private:
  float xyz[3];

  unsigned char rgba[4];
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QPOINTCOLORPAIR_H

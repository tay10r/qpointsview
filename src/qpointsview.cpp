#include <qpointsview/qpointsview.h>

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QVector>

namespace qpointsview {

class QPointsViewPrivate final
{
  friend QPointsView;

  float m_radiusNear = 2.0f;

  float m_radiusFar = 0.0f;

  QVector<QVector3D> m_points;

  QVector<QColor> m_colors;

  QMatrix4x4 m_viewMatrix;

  QMatrix4x4 m_projectionMatrix;
};

QPointsView::QPointsView(QWidget* parent)
  : QFrame(parent)
  , m_self(new QPointsViewPrivate())
{
  setStyleSheet("QFrame:focus { border-width: 1px; border-color: blue }");
}

QPointsView::~QPointsView()
{
  delete m_self;
}

void
QPointsView::setPointRadius(float nearRadius, float farRadius)
{
  m_self->m_radiusNear = nearRadius;

  m_self->m_radiusFar = farRadius;
}

void
QPointsView::setPoints(QVector<QVector3D>&& points)
{
  m_self->m_points = std::move(points);
}

void
QPointsView::setColors(QVector<QColor>&& colors)
{
  m_self->m_colors = std::move(colors);
}

void
QPointsView::setViewMatrix(const QMatrix4x4& viewMatrix)
{
  m_self->m_viewMatrix = viewMatrix;
}

void
QPointsView::setProjectionMatrix(const QMatrix4x4& projectionMatrix)
{
  m_self->m_projectionMatrix = projectionMatrix;
}

namespace {

bool
inNDCBounds(float value)
{
  return (value >= -1.0f) && (value <= 1.0f);
}

bool
inNDCBounds(const QVector3D& p)
{
  return inNDCBounds(p.x()) && inNDCBounds(p.y()) && inNDCBounds(p.z());
}

} // namespace

int
QPointsView::toScreenSpace(const QMatrix4x4& mvp,
                           int width,
                           int height,
                           const QVector3D* in,
                           int pointCount,
                           QVector3D* out)
{
  int resultCount = 0;

  QMatrix4x4 viewportTransform;

  viewportTransform.viewport(0, 0, width, height);

  for (int i = 0; i < pointCount; i++) {

    const QVector4D clipSpacePoint = mvp * QVector4D(in[i], 1.0f);

    const QVector3D ndcPoint = clipSpacePoint.toVector3DAffine();

    if (!inNDCBounds(ndcPoint))
      continue;

    const QVector4D screenPoint = viewportTransform * QVector4D(ndcPoint, 1.0f);

    out[resultCount] = screenPoint.toVector3D();

    resultCount++;
  }

  return resultCount;
}

} // namespace qpointsview

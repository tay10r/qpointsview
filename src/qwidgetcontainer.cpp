#include "qwidgetcontainer.h"

#include <QImage>
#include <QPainter>
#include <QWidget>

namespace qpointsview {

int
QWidgetContainer::width() const
{
  return m_widget->width();
}

int
QWidgetContainer::height() const
{
  return m_widget->height();
}

bool
QWidgetContainer::paintOntoTexture()
{
  if (!m_texture.isCreated()) {

    m_texture.create();

    const QSize size = m_widget->sizeHint();
    const int w = size.width();
    const int h = size.height();

    m_widget->resize(size);

    QImage image(w, h, QImage::Format_ARGB32_Premultiplied);

    QPainter painter;

    painter.begin(&image);

    m_widget->render(&painter);

    painter.end();

    m_texture.setData(image);
  }

  return true;
}

} // namespace qpointsview

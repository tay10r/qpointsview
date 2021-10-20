#pragma once

#ifndef QPOINTSVIEW_QFLYCAMERA_H
#define QPOINTSVIEW_QFLYCAMERA_H

#include <qpointsview/QAbstractCamera>

namespace qpointsview {

class QFlyCamera final : public QAbstractCamera
{
public:
  QFlyCamera(QObject* parent);
};

} // namespace qpointsview

#endif // QPOINTSVIEW_QFLYCAMERA_H

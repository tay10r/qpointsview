#include <qpointsview/QAbstractCamera>

namespace qpointsview {

QAbstractCamera::QAbstractCamera(QObject* parent)
  : QObject(parent)
{}

QAbstractCamera::~QAbstractCamera() = default;

} // namespace qpointsview

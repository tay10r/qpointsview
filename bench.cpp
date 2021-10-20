#include <QApplication>
#include <QMainWindow>
#include <QMatrix4x4>

#include <qpointsview/QFlyCamera>
#include <qpointsview/QPointColorPair>
#include <qpointsview/QPointsView>

#include <random>

namespace {

QVector<qpointsview::QPointColorPair>
generatePoints(int pointCount, int seedValue)
{
  std::seed_seq seed{ seedValue };

  std::mt19937 rng(seed);

  std::uniform_real_distribution<float> xDist(-100, 100);
  std::uniform_real_distribution<float> yDist(0.01, 0.4);
  std::uniform_real_distribution<float> zDist(-100, 100);

  std::uniform_real_distribution<float> rDist(0, 1);
  std::uniform_real_distribution<float> gDist(0, 1);
  std::uniform_real_distribution<float> bDist(0, 1);

  QVector<qpointsview::QPointColorPair> points(pointCount);

  for (int i = 0; i < pointCount; i++) {

    points[i].setPosition(xDist(rng), yDist(rng), zDist(rng));

    points[i].setColorf(rDist(rng), gDist(rng), bDist(rng));
  }

  return points;
}

} // namespace

int
main(int argc, char** argv)
{
  QApplication app(argc, argv);

  QMainWindow mainWindow;

  mainWindow.showMaximized();

  qpointsview::QPointsView pointsView(&mainWindow);

  qpointsview::QFlyCamera flyCamera(&pointsView);

  pointsView.setCamera(&flyCamera);

  mainWindow.setCentralWidget(&pointsView);

  // Setup points

  QVector<qpointsview::QPointColorPair> points =
    generatePoints(1'000'000, 1234);

  QObject::connect(&pointsView,
                   &qpointsview::QPointsView::contextInitialized,
                   [&pointsView, points]() {
                     pointsView.setPoints(points);
                     pointsView.update();
                   });

  // Setup view matrix

  QMatrix4x4 viewMatrix;

  viewMatrix.lookAt(QVector3D(2, 2, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

  pointsView.setViewMatrix(viewMatrix);

  // Setup projection matrix

  QMatrix4x4 projMatrix;

  projMatrix.perspective(45, 1.0f, 0.1, 100);

  pointsView.setProjectionMatrix(projMatrix);

  return app.exec();
}

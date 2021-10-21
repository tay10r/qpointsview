#include <QApplication>

#include <QLabel>

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

  QLabel* label = new QLabel("A test message.", &pointsView);

  label->setStyleSheet("font: 24pt");

  label->hide();

  pointsView.addWidget(label);

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

  auto projectionSetter = [&pointsView](int w, int h) {
    const float aspect = float(w) / h;

    QMatrix4x4 projMatrix;

    projMatrix.perspective(45, aspect, 0.1, 1000);

    pointsView.setProjectionMatrix(projMatrix);
  };

  QObject::connect(
    &pointsView, &qpointsview::QPointsView::resized, projectionSetter);

  return app.exec();
}

#include <QApplication>
#include <QMainWindow>

#include <qpointsview/QPointsView>

int
main(int argc, char** argv)
{

  QApplication app(argc, argv);

  QMainWindow mainWindow;

  mainWindow.showMaximized();

  qpointsview::QPointsView pointsView(&mainWindow);

  mainWindow.setCentralWidget(&pointsView);

  return app.exec();
}

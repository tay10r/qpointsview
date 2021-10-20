#include <qpointsview/qpointsview.h>

#include <qpointsview/QAbstractCamera>
#include <qpointsview/QPointColorPair>

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <QKeyEvent>
#include <QMouseEvent>

#include <QMatrix4x4>
#include <QTimer>
#include <QVector3D>
#include <QVector4D>
#include <QVector>

#include <cassert>

static void
initResources()
{
  Q_INIT_RESOURCE(shaders);
}

namespace qpointsview {

namespace {

class PointRenderer final
{
public:
  ~PointRenderer()
  {
    if (m_pointBuffer.isCreated())
      m_pointBuffer.destroy();
  }

  bool init()
  {
    if (!m_pointBuffer.create())
      return false;

    if (!m_program.create())
      return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/render_points.vert"))
      return false;

    if (!m_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/render_points.frag"))
      return false;

    if (!m_program.link())
      return false;

    m_mvpLocation = m_program.uniformLocation("mvp");

    m_positionLocation = m_program.attributeLocation("position");

    m_colorLocation = m_program.attributeLocation("color");

    return true;
  }

  bool setPoints(const QVector<QPointColorPair>& points)
  {
    if (!m_pointBuffer.bind())
      return false;

    static_assert(sizeof(QPointColorPair) == 16);

    const int bufferSize = points.size() * sizeof(QPointColorPair);

    m_pointBuffer.allocate(bufferSize);

    m_pointBuffer.write(0, points.data(), bufferSize);

    m_pointBuffer.release();

    return true;
  }

  void render(const QMatrix4x4& mvp)
  {
    const int bytesPerPoint = 16;

    m_program.bind();

    m_pointBuffer.bind();

    m_program.setUniformValue(m_mvpLocation, mvp);

    QOpenGLFunctions* functions = QOpenGLContext::currentContext()->functions();

    functions->glEnableVertexAttribArray(m_positionLocation);

    functions->glEnableVertexAttribArray(m_colorLocation);

    functions->glVertexAttribPointer(
      m_positionLocation, 3, GL_FLOAT, GL_FALSE, bytesPerPoint, (void*)0);

    functions->glVertexAttribPointer(
      m_colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, bytesPerPoint, (void*)12);

    functions->glDrawArrays(GL_POINTS, 0, pointCount());

    functions->glDisableVertexAttribArray(m_colorLocation);

    functions->glDisableVertexAttribArray(m_positionLocation);

    m_pointBuffer.release();

    m_program.release();
  }

  void setPointRadius(float near, float far)
  {
    m_radiusNear = near;

    m_radiusFar = far;
  }

  int pointCount() const
  {
    const int bufferSize = m_pointBuffer.size();

    if (bufferSize < 0)
      return 0;

    return bufferSize / sizeof(QPointColorPair);
  }

private:
  int m_mvpLocation = -1;

  int m_positionLocation = -1;

  int m_colorLocation = -1;

  float m_radiusNear = 2.0f;

  float m_radiusFar = 0.0f;

  QOpenGLBuffer m_pointBuffer;

  QOpenGLShaderProgram m_program;
};

class DragMotion final
{
public:
  void addMouseSample(int x, int y)
  {
    QPoint p(x, y);

    if (m_samples.size() >= 2) {
      m_samples[0] = m_samples[1];
      m_samples[1] = p;
    } else {
      m_samples.push_back(p);
    }
  }

  bool hasMotion() const noexcept
  {
    if (m_samples.size() < 2)
      return false;

    return m_samples[0] != m_samples[1];
  }

  QVector2D getMotion(const QSize& widgetSize) const
  {
    if (m_samples.size() < 2)
      return QVector2D(0, 0);

    const float w = widgetSize.width();
    const float h = widgetSize.height();

    const QPoint p0 = m_samples[0];
    const QPoint p1 = m_samples[1];

    const float du = (p1.x() - p0.x()) / w;
    const float dv = (p1.y() - p0.y()) / h;

    return QVector2D(du, dv);
  }

private:
  QVector<QPoint> m_samples;
};

class MoveState final
{
public:
  void handleKey(int key, bool state)
  {
    switch (key) {
      case Qt::Key_Up:
      case Qt::Key_W:
        m_up = state;
        break;
      case Qt::Key_Left:
      case Qt::Key_A:
        m_left = state;
        break;
      case Qt::Key_Down:
      case Qt::Key_S:
        m_down = state;
        break;
      case Qt::Key_Right:
      case Qt::Key_D:
        m_right = state;
        break;
    }
  }

  bool isMoving() const noexcept { return m_up || m_left || m_down || m_right; }

  QVector2D moveVector() const
  {
    QVector2D v(0, 0);

    if (m_up)
      v[1] += 1;

    if (m_left)
      v[0] -= 1;

    if (m_down)
      v[1] -= 1;

    if (m_right)
      v[0] += 1;

    return v.normalized();
  }

private:
  bool m_up = false;
  bool m_left = false;
  bool m_down = false;
  bool m_right = false;
};

} // namespace

class QPointsViewPrivate final
{
  friend QPointsView;

  bool m_contextInitialized = false;

  PointRenderer m_pointRenderer;

  QMatrix4x4 m_viewMatrix;

  QMatrix4x4 m_projectionMatrix;

  QTimer m_inputTimer;

  QAbstractCamera* m_camera = nullptr;

  int m_xMouse = 0;
  int m_yMouse = 0;

  std::unique_ptr<DragMotion> m_dragMotion;

  MoveState m_moveState;

  QPointsViewPrivate(QPointsView* parent)
    : m_inputTimer(parent)
  {
    const int inputRefreshRate = 60;

    m_inputTimer.setInterval(1000 / inputRefreshRate);

    QObject::connect(
      &m_inputTimer, &QTimer::timeout, parent, &QPointsView::updateInput);

    m_inputTimer.start();
  }
};

QPointsView::QPointsView(QWidget* parent)
  : QOpenGLWidget(parent)
  , m_self(new QPointsViewPrivate(this))
{
  initResources();

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

QPointsView::~QPointsView()
{
  makeCurrent();

  delete m_self;

  doneCurrent();
}

void
QPointsView::initializeGL()
{
  QOpenGLFunctions* functions = context()->functions();

  functions->glEnable(GL_DEPTH_TEST);

  functions->glEnable(GL_PROGRAM_POINT_SIZE);

  [[maybe_unused]] bool success = m_self->m_pointRenderer.init();

  assert(success);

  m_self->m_contextInitialized = true;

  QTimer::singleShot(0, [this]() { emit contextInitialized(); });
}

void
QPointsView::resizeGL(int w, int h)
{
  context()->functions()->glViewport(0, 0, w, h);
}

void
QPointsView::paintGL()
{
  QOpenGLFunctions* functions = context()->functions();

  functions->glClearColor(0, 0, 0, 1);

  functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const QMatrix4x4 mvp = m_self->m_projectionMatrix * m_self->m_viewMatrix;

  m_self->m_pointRenderer.render(mvp);
}

void
QPointsView::setCamera(QAbstractCamera* camera)
{
  m_self->m_camera = camera;
}

void
QPointsView::updateInput()
{
  if (m_self->m_dragMotion) {

    m_self->m_dragMotion->addMouseSample(m_self->m_xMouse, m_self->m_yMouse);

    const bool isDragging = m_self->m_dragMotion->hasMotion();

    const bool isMoving = m_self->m_moveState.isMoving();

    if (m_self->m_camera && (isDragging || isMoving)) {

      const QVector2D dragMotion = m_self->m_dragMotion->getMotion(size());

      m_self->m_camera->dragEvent(dragMotion.x(), dragMotion.y());

      const QVector2D moveMotion = m_self->m_moveState.moveVector();

      m_self->m_camera->moveEvent(moveMotion.x(), moveMotion.y());

      setViewMatrix(m_self->m_camera->viewMatrix());

      update();
    }
  }
}

void
QPointsView::mouseMoveEvent(QMouseEvent* mouseEvent)
{
  m_self->m_xMouse = mouseEvent->x();
  m_self->m_yMouse = mouseEvent->y();
  QOpenGLWidget::mouseMoveEvent(mouseEvent);
}

void
QPointsView::mousePressEvent(QMouseEvent* mouseEvent)
{
  if (mouseEvent->buttons() & Qt::LeftButton) {
    if (!m_self->m_dragMotion) {
      m_self->m_dragMotion.reset(new DragMotion());
      int x = m_self->m_xMouse;
      int y = m_self->m_yMouse;
      m_self->m_dragMotion->addMouseSample(x, y);
    }
  }

  QOpenGLWidget::mousePressEvent(mouseEvent);
}

void
QPointsView::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
  if (m_self->m_dragMotion)
    m_self->m_dragMotion.reset();

  QOpenGLWidget::mouseReleaseEvent(mouseEvent);
}

void
QPointsView::keyPressEvent(QKeyEvent* event)
{
  if (!event->isAutoRepeat())
    m_self->m_moveState.handleKey(event->key(), true);

  QOpenGLWidget::keyPressEvent(event);
}

void
QPointsView::keyReleaseEvent(QKeyEvent* event)
{
  if (!event->isAutoRepeat())
    m_self->m_moveState.handleKey(event->key(), false);

  QOpenGLWidget::keyReleaseEvent(event);
}

void
QPointsView::setPointRadius(float nearRadius, float farRadius)
{
  m_self->m_pointRenderer.setPointRadius(nearRadius, farRadius);
}

bool
QPointsView::setPoints(const QVector<QPointColorPair>& points)
{
  if (!m_self->m_contextInitialized)
    return false;

  makeCurrent();

  m_self->m_pointRenderer.setPoints(points);

  doneCurrent();

  return true;
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

} // namespace qpointsview

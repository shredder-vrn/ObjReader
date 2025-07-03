#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

#include "ObjReader/objreader.h"

namespace Viewer{

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    qDebug() << "Viewport :: Viewport : запустили конструктор";
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat :: CoreProfile);
    setFormat(format);


    m_camera = std::make_unique<CameraPer>();

}

void ViewportWidget::loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";
    makeCurrent();

    Model model;
    QFile file(filePath);

    if (!m_modelController.loadModel(filePath))
        qWarning() << "Не удалось загрузить модель";

    m_renderer.setModel(m_modelController.getModel());

    update();
    doneCurrent();
}




void ViewportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";

    m_viewportWidth = event->size().width();
    m_viewportHeight = event->size().height();
    m_camera->setViewportSize(m_viewportWidth, m_viewportHeight);
    QOpenGLWidget::resizeEvent(event);
}

void ViewportWidget :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";
    m_renderer.initialize();
    m_camera->setViewportSize(m_viewportWidth, m_viewportHeight);
}

void ViewportWidget::paintGL()
{
    qDebug() << "\n\nViewport :: paintGL : запустили метод paintGL";
    QMatrix4x4 model = m_modelController.getModelMatrix();
    QMatrix4x4 view = m_camera->viewMatrix();
    QMatrix4x4 proj = m_camera->projectionMatrix();
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "view: " << view;
    qDebug() << "proj: " << proj;
    qDebug() << "model: " << model;

    m_renderer.setMVPmatrix(proj * view * model);
    m_renderer.render();
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Viewport :: wheelEvent : запустили метод wheelEvent";
    const float delta = event->angleDelta().y() > 0 ? 0.5f : -0.5f;
    m_camera->zoom(delta);
    update();
}

void ViewportWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        m_lastMousePos = event->pos();
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::RightButton))
        return;

    const QPoint delta = event->pos() - m_lastMousePos;
    m_camera->rotateAroundTarget(-delta.x(), delta.y());

    m_lastMousePos = event->pos();
    update();
}

void ViewportWidget::switchToPerspective()
{
    if (m_camera->type() == CameraType::Perspective)
        return;
    auto newCam = std::make_unique<CameraPer>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);

    update();
}

void ViewportWidget::switchToOrthographic()
{
    if (m_camera->type() == CameraType::Orthographic)
        return;
    auto newCam = std::make_unique<CameraOrt>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);

    update();
}
}

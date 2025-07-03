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
}

void ViewportWidget::loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";
    makeCurrent();

    Model model;
    QFile file(filePath);





    m_scene.loadModel(filePath);





    update();
    doneCurrent();
}



void ViewportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";
    m_scene.resize(event->size().width(), event->size().height());
    QOpenGLWidget::resizeEvent(event);
}

void ViewportWidget :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";
    m_scene.initialize();
}

void ViewportWidget::paintGL()
{
    qDebug() << "\n\nViewport :: paintGL : запустили метод paintGL";
    m_scene.render();
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Viewport :: wheelEvent : запустили метод wheelEvent";
    const float delta = event->angleDelta().y() > 0 ? 0.5f : -0.5f;
    m_scene.zoom(delta);
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
    m_scene.rotateAroundTarget(-delta.x(), delta.y());
    m_lastMousePos = event->pos();
    update();
}

void ViewportWidget::switchToPerspective()
{
    m_scene.switchToPerspective();
    update();
}

void ViewportWidget::switchToOrthographic()
{
    m_scene.switchToOrthographic();
    update();
}
}

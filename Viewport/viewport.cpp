#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

#include <ObjReader/objreader.h>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{
    qDebug() << "Viewport :: Viewport : запустили конструктор";
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat :: CoreProfile);
    setFormat(format);
}

void Viewport :: loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";
    makeCurrent();

    Model model;
    QFile file(filePath);

    scene.loadModel(filePath);

    update();
    doneCurrent();
}

void Viewport::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";
    scene.resize(event->size().width(), event->size().height());
    QOpenGLWidget::resizeEvent(event);
}

void Viewport :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";
    scene.initialize();
}

void Viewport::paintGL()
{
    qDebug() << "\n\nViewport :: paintGL : запустили метод paintGL";
    scene.render();
}

void Viewport::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Viewport :: wheelEvent : запустили метод wheelEvent";
    float delta = event->angleDelta().y() > 0 ? 0.5f : -0.5f;
    scene.getCamera().zoom(delta);
    update();
}

void Viewport::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        lastMousePos = event->pos();
        qDebug() << lastMousePos;}
}

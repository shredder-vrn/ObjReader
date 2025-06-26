#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

Viewport::~Viewport() = default;

void Viewport::initializeGL() {
    scene.initialize();
}

void Viewport::paintGL() {
    scene.render();
}

void Viewport::loadModel(const QString& filePath) {
    scene.setModelPath(filePath);
    scene.loadModel();
    update();
}

void Viewport::mousePressEvent(QMouseEvent* event) {
    lastMousePos = event->pos();
}

void Viewport::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        int dx = event->x() - lastMousePos.x();
        int dy = event->y() - lastMousePos.y();
        scene.rotate(dx * 0.5f, dy * 0.5f);
        update();
    }
    lastMousePos = event->pos();
}

#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{

    qDebug() << "Viewport :: Viewport : запустили конструктор";
    qDebug() << "viewportLastMousePos" << viewportLastMousePos;

    QSurfaceFormat format;

    format.setVersion(3, 3);


    format.setProfile(QSurfaceFormat :: CoreProfile);

    setFormat(format);

    qDebug() << "Viewport :: Viewport : конструктор отработал";
}


void Viewport :: loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";

    makeCurrent();

    scene.setModelPath(filePath);

    scene.loadModel();

    update();

    doneCurrent();

    qDebug() << "Viewport :: loadModel : метод loadModel отработал";

}

void Viewport :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";

    //initializeOpenGLFunctions();
    scene.initialize();

    qDebug() << "Viewport :: initializeGL : метод initializeGL отработал";
}


void Viewport::paintGL()
{
    qDebug() << "Viewport :: paintGL : запустили метод paintGL";

    scene.render();

    qDebug() << "Viewport :: paintGL : метод paintGL отработал";
}


void Viewport :: mousePressEvent(QMouseEvent* event)
{
    qDebug() << "Viewport :: mousePressEvent : запустили метод mousePressEvent";

    viewportLastMousePos = event->pos();

    qDebug() << "Viewport :: mousePressEvent : метод mousePressEvent отработал";
}

void Viewport :: mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "Viewport :: mouseMoveEvent : запустили метод mouseMoveEvent";

    if (event->buttons() & Qt::LeftButton) {
        int dx = event->x() - viewportLastMousePos.x();
        int dy = event->y() - viewportLastMousePos.y();

        scene.rotate(dx * 0.5f, dy * 0.5f);
        update();
    }
    viewportLastMousePos = event->pos();

    qDebug() << "Viewport :: mouseMoveEvent : метод mouseMoveEvent отработал";
}


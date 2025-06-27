#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{
<<<<<<< Updated upstream
=======
    qDebug() << "Viewport :: Viewport : запустили конструктор";
    qDebug() << "viewportLastMousePos" << viewportLastMousePos;

>>>>>>> Stashed changes
    QSurfaceFormat format;

    format.setVersion(3, 3);
<<<<<<< Updated upstream
    format.setProfile(QSurfaceFormat::CoreProfile);
=======

    format.setProfile(QSurfaceFormat :: CoreProfile);

>>>>>>> Stashed changes
    setFormat(format);

    qDebug() << "Viewport :: Viewport : конструктор отработал";
}

<<<<<<< Updated upstream
Viewport::~Viewport() = default;

void Viewport::initializeGL() {
=======
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

    initializeOpenGLFunctions();
>>>>>>> Stashed changes
    scene.initialize();

    qDebug() << "Viewport :: initializeGL : метод initializeGL отработал";
}

<<<<<<< Updated upstream
void Viewport::paintGL() {
=======
void Viewport::paintGL()
{
    qDebug() << "Viewport :: paintGL : запустили метод paintGL";

>>>>>>> Stashed changes
    scene.render();

    qDebug() << "Viewport :: paintGL : метод paintGL отработал";
}

<<<<<<< Updated upstream
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
=======
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

>>>>>>> Stashed changes
        scene.rotate(dx * 0.5f, dy * 0.5f);
        update();
    }
    viewportLastMousePos = event->pos();

    qDebug() << "Viewport :: mouseMoveEvent : метод mouseMoveEvent отработал";
}
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes

#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

//! REVIEW: <ObjReader/objreader.h> -> "ObjReader/objreader.h"
#include <ObjReader/objreader.h>

namespace Viewer{

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    qDebug() << "Viewport :: Viewport : запустили конструктор";
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat :: CoreProfile);
    setFormat(format);
}

//! REVIEW: TODO: decomposition
void ViewportWidget::loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";
    makeCurrent();

    Model model;
    QFile file(filePath);

    scene.loadModel(filePath);

    update();
    doneCurrent();
}

void ViewportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";
    scene.resize(event->size().width(), event->size().height());
    QOpenGLWidget::resizeEvent(event);
}

void ViewportWidget :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";
    //! REVIEW:
    //    if (scenePtr != nullptr) {
    //        delete scenePtr;
    //        scenePtr = new Scene();
    //    }
    scene.initialize();
}

void ViewportWidget::paintGL()
{
    qDebug() << "\n\nViewport :: paintGL : запустили метод paintGL";
    // Q_ASSERT(scenePtr != nullptr);
    scene.render();
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Viewport :: wheelEvent : запустили метод wheelEvent";
    //! REVIEW: const
    float delta = event->angleDelta().y() > 0 ? 0.5f : -0.5f;
    //! REVIEW: TODO: demetra rule
    scene.getCamera().zoom(delta);
    update();
}

void ViewportWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        lastMousePos = event->pos();
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
    //! REVIEW:
    if (!(event->buttons() & Qt::RightButton))
        return;

    //! REVIEW: const
    QPoint delta = event->pos() - lastMousePos;
    //! REVIEW: demetra rule
    scene.getCamera().rotateAroundTarget(-delta.x(), delta.y());
    lastMousePos = event->pos();
    update();
}
}

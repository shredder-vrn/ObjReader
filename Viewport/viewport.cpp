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
    scene.setModelPath(filePath);

    Model model;
    QFile file(filePath);

    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filePath;
        return;}
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qCritical() << "Не удалось открыть файл для чтения:" << filePath;
        return;}
    if (!parseObj(filePath, model)){
        qCritical() << "Ошибка при разборе файла OBJ.";
        return;}

    scene.loadModel(model);

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

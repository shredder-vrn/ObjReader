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
    qDebug() << "Viewport :: Viewport : конструктор отработал, все преобразования обработаны.";
}

void Viewport :: loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";
    makeCurrent();
    scene.setModelPath(filePath);

    Model model;
    QFile file(filePath);

    if (!file.exists())
        qCritical() << "Файл не найден:" << filePath;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qCritical() << "Не удалось открыть файл для чтения:" << filePath;
    QTextStream in(&file);
    if (!parseTokens(in, model))
        qCritical() << "Ошибка при разборе файла OBJ.";

    scene.loadModel(model);

    update();
    doneCurrent();
    qDebug() << "Viewport :: loadModel : метод loadModel отработал, все преобразования обработаны.";
}

void Viewport::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";
    scene.resize(event->size().width(), event->size().height());
    QOpenGLWidget::resizeEvent(event);
    qDebug() << "Viewport :: resizeEvent : метод resizeEvent отработал, все преобразования обработаны.";
}

void Viewport :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";
    scene.initialize();
    qDebug() << "Viewport :: initializeGL : метод initializeGL отработал, все преобразования обработаны.";
}

void Viewport::paintGL()
{
    qDebug() << "\n\nViewport :: paintGL : запустили метод paintGL";
    scene.render();
    qDebug() << "Viewport :: paintGL : метод paintGL отработал, все преобразования обработаны";
}

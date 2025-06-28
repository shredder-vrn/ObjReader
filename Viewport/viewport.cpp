#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>

#include <ObjReader/objreader.h>

Viewport::Viewport(QWidget* parent) : QOpenGLWidget(parent)
{
    qDebug() << "Viewport :: Viewport : запустили конструктор";

    qDebug() << "viewportLastMousePos" << viewportLastMousePos;

    QSurfaceFormat format;

    format.setVersion(3, 3);


    format.setProfile(QSurfaceFormat :: CoreProfile);

    setFormat(format);

    qDebug() << "Viewport :: Viewport : конструктор отработал, все преобразования обработаны.";
}


void Viewport :: loadModel(const QString &filePath)
{
    qDebug() << "Viewport :: loadModel : запустили метод loadModel";

    qDebug() << "filePath" <<  filePath;

    qDebug() << "Запускаем makeCurrent() - активирует контекст OpenGL для текущего потока";

    makeCurrent();

    scene.setModelPath(filePath);

    Model model;

    QFile file(filePath);

    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filePath;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл для чтения:" << filePath;
    }

    QTextStream in(&file);
    if (!parseTokens(in, model)) {
        qCritical() << "Ошибка при разборе файла OBJ.";
    }

    qDebug() << "model.vertices" << model.vertices;
    qDebug() << "model.vertices" << model.textureVertices;
    qDebug() << "model.vertices" << model.normals;
    qDebug() << "model.faceVertexIndices" << model.faceVertexIndices;
    qDebug() << "model.faceTextureVertexIndices" << model.faceTextureVertexIndices;
    qDebug() << "model.faceNormalIndices" << model.faceNormalIndices;
    qDebug() << "model.polygonStarts" << model.polygonStarts;



    scene.loadModel(model);

    update();
    qDebug() << "update() - Перерисовываем после загрузки новой модели";

    doneCurrent();
    qDebug() << "doneCurrent() - Отвязываем текущий контекст makeCurrent(), от текущего потока";

    qDebug() << "Viewport :: loadModel : метод loadModel отработал, все преобразования обработаны.";
}

void Viewport :: initializeGL()
{
    qDebug() << "Viewport :: initializeGL : запустили метод initializeGL";

    scene.initialize();

    qDebug() << "Viewport :: initializeGL : метод initializeGL отработал, все преобразования обработаны.";
}


void Viewport::paintGL()
{
    qDebug() << "";

    qDebug() << "Viewport :: paintGL : запустили метод paintGL";

    scene.render();

    qDebug() << "Viewport :: paintGL : метод paintGL отработал, все преобразования обработаны";
    qDebug() << "";

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


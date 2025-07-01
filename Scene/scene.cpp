#include "scene.h"

void Scene :: initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    renderer.initialize();
    camera.setViewportSize(sceneWidth, sceneHeight);
}

void Scene::loadModel(const QString &filePath)
{
    qDebug() << "Scene : : loadModel : запустили метод loadModel";

    if (!modelController.loadModel(filePath))
        qWarning() << "Не удалось загрузить модель";

    renderer.setModel(modelController.getModel());
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "modelController.getModelMatrix(): " << modelController.getModelMatrix();
    qDebug() << "camera.getViewMatrix(): " << camera.getViewMatrix();
    qDebug() << "camera.getProjectionMatrix(): " << camera.getProjectionMatrix();

    QMatrix4x4 matrix;

    matrix = modelController.getModelMatrix();

    matrix.rotate(30, {1, 1, 0});

    renderer.setMVPmatrix(camera.getProjectionMatrix() * camera.getViewMatrix() * matrix);

    renderer.render();
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";
    sceneWidth = w;
    sceneHeight = h;
    camera.setViewportSize(w, h);
}


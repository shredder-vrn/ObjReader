#include "scene.h"



void Scene :: setModelPath(const QString &path)
{
    qDebug() << "Scene :: setModelPath : запустили метод setModelPath";
    modelPath = path;
}

void Scene :: initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    renderer.initialize();
    camera.setViewportSize(sceneWidth, sceneHeight);
}

void Scene :: loadModel(Model &model)
{
    qDebug() << "Scene : : loadModel : запустили метод loadModel";
    renderer.setModel(model);
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";

    qDebug() << "modelController.getModelMatrix(): " << modelController.getModelMatrix();
    qDebug() << "camera.getViewMatrix(): " << camera.getViewMatrix();
    qDebug() << "camera.getProjectionMatrix(): " << camera.getProjectionMatrix();

    renderer.setMVPmatrix(camera.getProjectionMatrix() * camera.getViewMatrix() * modelController.getModelMatrix());

    renderer.render();
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";
    sceneWidth = w;
    sceneHeight = h;
    camera.setViewportSize(w, h);
}


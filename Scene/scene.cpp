#include "scene.h"



void Scene :: setModelPath(const QString &path)
{
    qDebug() << "Scene :: setModelPath : запустили метод setModelPath";
    modelPath = path;
    qDebug() << "modelPath 'после'" << modelPath;
    qDebug() << "Scene :: setModelPath : метод setModelPath отработал, все преобразования обработаны.";
}

void Scene :: initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    renderer.initialize();
    camera.setViewportSize(sceneWidth, sceneHeight);
    qDebug() << "Scene :: initialize : метод initialize отработал, все преобразования обработаны.";
}

void Scene :: loadModel(Model &model)
{
    qDebug() << "Scene :: loadModel : запустили метод loadModel";
    renderer.setModel(model);
    qDebug() << "Scene :: loadModel : метод loadModel отработал, все преобразования обработаны.";
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";
    // renderer.setViewProjectionMatrix(camera.getViewMatrix() * camera.getProjectionMatrix());



    renderer.render();
    qDebug() << "Scene :: render : метод render отработал, все преобразования обработаны";
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";
    sceneWidth = w;
    sceneHeight = h;
    camera.setViewportSize(w, h);
    qDebug() << "Scene :: resize : метод resize отработал";
}

bool Scene :: hasModel() const
{
    qDebug() << "Scene :: hasModel : запустили метод hasModel";
    bool result = modelController.getModel().vertices.size() > 0;
    return result;
    qDebug() << "Scene :: hasModel : метод hasModel отработал";
}

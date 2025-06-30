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

bool Scene :: hasModel() const
{
    qDebug() << "Scene :: hasModel : запустили метод hasModel";
    bool result = modelController.getModel().vertices.size() > 0;
    return result;
}

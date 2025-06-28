#include "scene.h"


Scene :: Scene() : modelController(), renderer()
{
    qDebug() << "Scene :: Scene : запустили конструктор";

    qDebug() << "modelPath" << modelPath;

    qDebug() << "Scene :: Scene : конструктор отработал";

}

void Scene :: setModelPath(const QString &path)
{
    qDebug() << "Scene :: setModelPath : запустили метод setModelPath";

    modelPath = path;

    qDebug() << "Scene :: setModelPath : метод setModelPath отработал";
}

void Scene :: initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";

    renderer.initialize();

    qDebug() << "Scene :: initialize : метод initialize отработал";

}


void Scene :: loadModel()
{
    qDebug() << "Scene :: loadModel : запустили метод loadModel";

    renderer.initializeModel(Model());

    qDebug() << "Scene :: loadModel : метод loadModel отработал";
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";

    renderer.render();

    qDebug() << "Scene :: render : метод render отработал";
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";

    camera.setViewportSize(w, h);
    renderer.resize(w, h);

    qDebug() << "Scene :: resize : метод resize отработал";
}

void Scene :: rotate(float dx, float dy)
{
    qDebug() << "Scene :: rotate : запустили метод rotate";

    modelController.rotate(dx, dy);

    qDebug() << "Scene :: rotate : метод rotate отработал";
}


bool Scene :: hasModel() const
{
    qDebug() << "Scene :: hasModel : запустили метод hasModel";

    bool result = modelController.getModel().vertices.size() > 0;
    return result;

    qDebug() << "Scene :: hasModel : метод hasModel отработал";
}

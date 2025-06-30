#include "scene.h"


Scene :: Scene() : modelController(), renderer()
{
    qDebug() << "Scene :: Scene : запустили конструктор";

    qDebug() << "modelPath" << modelPath;
    qDebug() << "sceneWidth" << sceneWidth;
    qDebug() << "sceneHeight" << sceneHeight;

    qDebug() << "Scene :: Scene : конструктор отработал, все преобразования обработаны.";

}

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

    qDebug() << "model.vertices" << model.vertices;
    qDebug() << "model.vertices" << model.textureVertices;
    qDebug() << "model.vertices" << model.normals;
    qDebug() << "model.faceVertexIndices" << model.faceVertexIndices;
    qDebug() << "model.faceTextureVertexIndices" << model.faceTextureVertexIndices;
    qDebug() << "model.faceNormalIndices" << model.faceNormalIndices;
    qDebug() << "model.polygonStarts" << model.polygonStarts;

    renderer.setModel(model);

    qDebug() << "Scene :: loadModel : метод loadModel отработал, все преобразования обработаны.";
}

void Scene :: render()
{
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "camera.getViewMatrix()" << camera.getViewMatrix();
    qDebug() << "camera.getProjectionMatrix()" << camera.getProjectionMatrix();
   // qDebug() << "setViewProjectionMatrix(camera.getViewMatrix() * camera.getProjectionMatrix())" << camera.getViewMatrix() * camera.getProjectionMatrix();
    qDebug() << "camera.getProjectionMatrix() * camera.getViewMatrix()" << camera.getProjectionMatrix() * camera.getViewMatrix();
    qDebug() << "camera.getViewMatrix() * camera.getProjectionMatrix()" << camera.getViewMatrix() * camera.getProjectionMatrix();



    renderer.setViewProjectionMatrix(camera.getProjectionMatrix() * camera.getViewMatrix());
    renderer.render();

    qDebug() << "Scene :: render : метод render отработал, все преобразования обработаны";
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";

    qDebug() << "w" << w;
    qDebug() << "h" << h;

    sceneWidth = w;
    sceneHeight = h;

    qDebug() << "sceneWidth" << sceneWidth;
    qDebug() << "sceneHeight" << sceneHeight;

    camera.setViewportSize(w, h);


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

#include "scene.h"

void Scene::initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    renderer.initialize();
    camera.setViewportSize(sceneWidth, sceneHeight);
}

//! REVIEW: decompose
void Scene::loadModel(const QString &filePath)
{
    qDebug() << "Scene : : loadModel : запустили метод loadModel";

    if (!modelController.loadModel(filePath))
        qWarning() << "Не удалось загрузить модель";

    renderer.setModel(modelController.getModel());
}


//! REVIEW:
/**
    Viewport
        - keeps navigation
        - keeps camera
        - keeps gl objects
        - draws gl objects
    GLObject
        - incapsulates any drawable object
    Camera
        - incapsulates any computations of model view matrix
    Navigation
        - incapsulates how mouse movements + keys converts to cameras rotation and translation

**/

void Scene::render()
{
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "modelController.getModelMatrix(): " << modelController.getModelMatrix();
    qDebug() << "camera.getViewMatrix(): " << camera.getViewMatrix();
    qDebug() << "camera.getProjectionMatrix(): " << camera.getProjectionMatrix();

    QMatrix4x4 matrix;

    matrix = modelController.getModelMatrix();
    // QMatrix4x4 matrix = modelController.getModelMatrix();

    matrix.rotate(30, {1, 1, 0});

    renderer.setMVPmatrix(camera.getProjectionMatrix() * camera.getViewMatrix() * matrix);

    renderer.render();
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";
    //! REVIEW:
    //! int sceneWidth; -> int width() const { return camera.width(); }
    //! scene.width();

    sceneWidth = w;
    sceneHeight = h;
    camera.setViewportSize(w, h);
}


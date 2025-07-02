#include "scene.h"

namespace Viewer{

void Scene::initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    m_renderer.initialize();
    m_camera.setViewportSize(m_sceneWidth, m_sceneHeight);
}

//! REVIEW: decompose
void Scene::loadModel(const QString &filePath)
{
    qDebug() << "Scene : : loadModel : запустили метод loadModel";

    if (!m_modelController.loadModel(filePath))
        qWarning() << "Не удалось загрузить модель";

    m_renderer.setModel(m_modelController.getModel());
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
    qDebug() << "modelController.getModelMatrix(): " << m_modelController.getModelMatrix();
    qDebug() << "camera.getViewMatrix(): " << m_camera.viewMatrix();
    qDebug() << "camera.getProjectionMatrix(): " << m_camera.projectionMatrix();

    m_renderer.setMVPmatrix(m_camera.projectionMatrix() * m_camera.viewMatrix() * m_modelController.getModelMatrix());
    m_renderer.render();
}

void Scene :: resize(int w, int h)
{
    qDebug() << "Scene :: resize : запустили метод resize";
    //! REVIEW:
    //! int sceneWidth; -> int width() const { return camera.width(); }
    //! scene.width();

    m_sceneWidth = w;
    m_sceneHeight = h;
    m_camera.setViewportSize(w, h);
}

}


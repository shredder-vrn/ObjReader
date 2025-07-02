#include "scene.h"

namespace Viewer{

void Scene::initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    m_renderer.initialize();
    m_camera.setViewportSize(m_sceneWidth, m_sceneHeight);
}

void Scene::loadModel(const QString &filePath)
{
    qDebug() << "Scene : : loadModel : запустили метод loadModel";

    if (!m_modelController.loadModel(filePath))
        qWarning() << "Не удалось загрузить модель";

    m_renderer.setModel(m_modelController.getModel());
}

void Scene::render()
{
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "modelController.getModelMatrix(): " << m_modelController.getModelMatrix();
    qDebug() << "camera.getViewMatrix(): " << m_camera.viewMatrix();
    qDebug() << "camera.getProjectionMatrix(): " << m_camera.projectionMatrix();

    m_renderer.setMVPmatrix(m_camera.projectionMatrix() * m_camera.viewMatrix() * m_modelController.getModelMatrix());
    m_renderer.render();
}

void Scene :: resize(const int width, const int height)
{
    qDebug() << "Scene :: resize : запустили метод resize";

    m_sceneWidth = width;
    m_sceneHeight = height;
    m_camera.setViewportSize(width, height);
}

}


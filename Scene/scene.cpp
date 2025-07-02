#include "scene.h"


namespace Viewer{

Scene::Scene()
{
    m_camera = std::make_unique<CameraPer>();
}

void Scene::initialize()
{
    qDebug() << "Scene :: initialize : запустили метод initialize";
    m_renderer.initialize();
    m_camera->setViewportSize(m_sceneWidth, m_sceneHeight);
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
    QMatrix4x4 model = m_modelController.getModelMatrix();
    QMatrix4x4 view = m_camera->viewMatrix();
    QMatrix4x4 proj = m_camera->projectionMatrix();
    qDebug() << "Scene :: render : запустили метод render";
    qDebug() << "view: " << view;
    qDebug() << "proj: " << proj;
    qDebug() << "model: " << model;

    m_renderer.setMVPmatrix(proj * view * model);
    m_renderer.render();
}

void Scene :: resize(const int width, const int height)
{
    qDebug() << "Scene :: resize : запустили метод resize";

    m_sceneWidth = width;
    m_sceneHeight = height;
    m_camera->setViewportSize(width, height);
}

void Scene::switchToPerspective()
{
    if (m_camera->type() == CameraType::Perspective)
        return;
    auto newCam = std::make_unique<CameraPer>();
    newCam->setViewportSize(m_sceneWidth, m_sceneHeight);
    m_camera = std::move(newCam);
}

void Scene::switchToOrthographic()
{
    if (m_camera->type() == CameraType::Orthographic)
        return;

    auto newCam = std::make_unique<CameraOrt>();
    newCam->setViewportSize(m_sceneWidth, m_sceneHeight);
    m_camera = std::move(newCam);
}

void Scene::zoom(float delta)
{
    m_camera->zoom(delta);
}

void Scene::rotateAroundTarget(float deltaX, float deltaY)
{
    m_camera->rotateAroundTarget(deltaX, deltaY);
}

}


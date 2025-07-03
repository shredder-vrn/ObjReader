#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>
#include <cmath>

#include "ObjReader/objreader.h"

namespace Viewer{

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent)
{
    qDebug() << "Viewport :: Viewport : запустили конструктор";
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat :: CoreProfile);
    setFormat(format);
    m_camera = std::make_unique<CameraPer>();
}

void Viewer::ViewportWidget::setModels(const QVector<Model*>& models, const QVector<QMatrix4x4>& transforms)
{
    m_models = models;
    m_modelTransforms = transforms;

    update();
}

bool Viewer::ViewportWidget::loadTextureForModel(const QString& texturePath, int modelIndex)
{
    if (modelIndex < 0 || modelIndex >= m_models.size())
        return false;

    return m_renderer.loadTexture(*m_models[modelIndex], texturePath);
}

void ViewportWidget::resizeEvent(QResizeEvent *event)
{
    qDebug() << "Viewport :: resizeEvent : запустили метод resizeEvent";

    m_viewportWidth = event->size().width();
    m_viewportHeight = event->size().height();
    m_camera->setViewportSize(m_viewportWidth, m_viewportHeight);
    QOpenGLWidget::resizeEvent(event);
}

void Viewer::ViewportWidget::initializeGL()
{
    m_renderer.initialize();
    m_camera->setViewportSize(width(), height());

    //createGrid(10.0f, 1.0f);
    //createWorldAxes(1.0f);
    //createLocalAxes(1.0f);

    m_renderer.initializeModel(m_grid);
    m_renderer.initializeModel(m_worldAxes);
    m_renderer.initializeModel(m_cameraTarget);
    m_renderer.initializeModel(m_localAxes);
}

void Viewer::ViewportWidget::paintGL()
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = m_camera->viewMatrix();
    QMatrix4x4 proj = m_camera->projectionMatrix();

    QMatrix4x4 gridMVP = proj * view;
    m_renderer.render(m_grid, gridMVP);

    for (int i = 0; i < m_models.size(); ++i) {
        if (!m_models[i] || !m_models[i]->isValid()) {
            qDebug() << "[ERROR] Модель #" << i << "невалидна";
            continue;
        }

        QMatrix4x4 mvp = proj * view * m_modelTransforms[i];
        m_renderer.render(*m_models[i], mvp);
    }
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
    qDebug() << "Viewport :: wheelEvent : запустили метод wheelEvent";
    const float delta = event->angleDelta().y() > 0 ? 0.5f : -0.5f;
    m_camera->zoom(delta);
    update();
}

void ViewportWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        m_lastMousePos = event->pos();
}

void ViewportWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::RightButton))
        return;
    const QPoint delta = event->pos() - m_lastMousePos;
    m_camera->rotateAroundTarget(-delta.x(), delta.y());
    m_lastMousePos = event->pos();
    update();
}

void ViewportWidget::createGrid(float size, float step)
{
    float halfSize = size * 0.5f;
    int linesX = static_cast<int>(size / step) + 1;
    int linesZ = static_cast<int>(size / step) + 1;
    m_grid.vertices.reserve((linesX + linesZ) * 2);
    for (int i = 0; i < linesX; ++i) {
        float x = -halfSize + i * step;
        m_grid.vertices.append(QVector3D(x, 0.0f, -halfSize));
        m_grid.vertices.append(QVector3D(x, 0.0f,  halfSize));
    }
    for (int i = 0; i < linesZ; ++i) {
        float z = -halfSize + i * step;
        m_grid.vertices.append(QVector3D(-halfSize, 0.0f, z));
        m_grid.vertices.append(QVector3D( halfSize, 0.0f, z));
    }
    for (int i = 0; i < m_grid.vertices.size(); i += 2) {
        m_grid.faceVertexIndices.append(i);
        m_grid.faceVertexIndices.append(i + 1);
        m_grid.polygonStarts.append(i);
    }
    m_renderer.initializeModel(m_grid);
}

void ViewportWidget::createWorldAxes(float size)
{
    m_worldAxes.vertices.append(QVector3D(0, 0, 0));
    m_worldAxes.vertices.append(QVector3D(size, 0, 0));
    m_worldAxes.vertices.append(QVector3D(0, 0, 0));
    m_worldAxes.vertices.append(QVector3D(0, size, 0));
    m_worldAxes.vertices.append(QVector3D(0, 0, 0));
    m_worldAxes.vertices.append(QVector3D(0, 0, size));
    for (int i = 0; i < 6; i += 2) {
        m_worldAxes.faceVertexIndices.append(i);
        m_worldAxes.faceVertexIndices.append(i + 1);
        m_worldAxes.polygonStarts.append(i);
    }
    m_renderer.initializeModel(m_worldAxes);
}

void ViewportWidget::createLocalAxes(float size)
{
    m_localAxes.vertices.append(QVector3D(0, 0, 0));
    m_localAxes.vertices.append(QVector3D(size, 0, 0));
    m_localAxes.vertices.append(QVector3D(0, 0, 0));
    m_localAxes.vertices.append(QVector3D(0, size, 0));
    m_localAxes.vertices.append(QVector3D(0, 0, 0));
    m_localAxes.vertices.append(QVector3D(0, 0, size));
    for (int i = 0; i < 6; i += 2) {
        m_localAxes.faceVertexIndices.append(i);
        m_localAxes.faceVertexIndices.append(i + 1);
        m_localAxes.polygonStarts.append(i);
    }
    m_renderer.initializeModel(m_localAxes);
}

void ViewportWidget::switchToPerspective()
{
    if (m_camera->type() == CameraType::Perspective)
        return;
    auto newCam = std::make_unique<CameraPer>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);
    update();
}

void ViewportWidget::switchToOrthographic()
{
    if (m_camera->type() == CameraType::Orthographic)
        return;
    auto newCam = std::make_unique<CameraOrt>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);
    update();
}

}

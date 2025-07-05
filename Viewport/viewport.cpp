#include "viewport.h"

#include <QMouseEvent>
#include <QDebug>
#include <QtMath>
#include <limits>

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

void ViewportWidget::setModels(const QVector<Model*>& models, const QVector<QMatrix4x4>& transforms)
{
    m_models = models;
    m_modelTransforms = transforms;

    update();
}

bool ViewportWidget::loadTextureForModel(const QString& texturePath, int modelIndex)
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

void ViewportWidget::initializeGL()
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

void ViewportWidget::paintGL()
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
    m_grid.setVertices().reserve((linesX + linesZ) * 2);
    for (int i = 0; i < linesX; ++i) {
        float x = -halfSize + i * step;
        m_grid.setVertices().append(QVector3D(x, 0.0f, -halfSize));
        m_grid.setVertices().append(QVector3D(x, 0.0f,  halfSize));
    }
    for (int i = 0; i < linesZ; ++i) {
        float z = -halfSize + i * step;
        m_grid.setVertices().append(QVector3D(-halfSize, 0.0f, z));
        m_grid.setVertices().append(QVector3D( halfSize, 0.0f, z));
    }
    for (int i = 0; i < m_grid.vertices().size(); i += 2) {
        m_grid.setFaceVertexIndices().append(i);
        m_grid.setFaceVertexIndices().append(i + 1);
        m_grid.setPolygonStarts().append(i);
    }
    m_renderer.initializeModel(m_grid);
}

void ViewportWidget::createWorldAxes(float size)
{
    m_worldAxes.setVertices().append(QVector3D(0, 0, 0));
    m_worldAxes.setVertices().append(QVector3D(size, 0, 0));
    m_worldAxes.setVertices().append(QVector3D(0, 0, 0));
    m_worldAxes.setVertices().append(QVector3D(0, size, 0));
    m_worldAxes.setVertices().append(QVector3D(0, 0, 0));
    m_worldAxes.setVertices().append(QVector3D(0, 0, size));
    for (int i = 0; i < 6; i += 2) {
        m_worldAxes.setFaceVertexIndices().append(i);
        m_worldAxes.setFaceVertexIndices().append(i + 1);
        m_worldAxes.setPolygonStarts().append(i);
    }
    m_renderer.initializeModel(m_worldAxes);
}

void ViewportWidget::createLocalAxes(float size)
{
    m_localAxes.setVertices().append(QVector3D(0, 0, 0));
    m_localAxes.setVertices().append(QVector3D(size, 0, 0));
    m_localAxes.setVertices().append(QVector3D(0, 0, 0));
    m_localAxes.setVertices().append(QVector3D(0, size, 0));
    m_localAxes.setVertices().append(QVector3D(0, 0, 0));
    m_localAxes.setVertices().append(QVector3D(0, 0, size));
    for (int i = 0; i < 6; i += 2) {
        m_localAxes.setFaceVertexIndices().append(i);
        m_localAxes.setFaceVertexIndices().append(i + 1);
        m_localAxes.setPolygonStarts().append(i);
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

void ViewportWidget::fitToView()
{
    if (m_models.isEmpty() || m_modelTransforms.isEmpty()) {
        qDebug() << "[INFO] Нет моделей для Fit to View";
        return;
    }

    QVector3D minBound(std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max());

    QVector3D maxBound(-std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max());

    for (int i = 0; i < m_models.size(); ++i) {
        const Model* model = m_models[i];
        const QMatrix4x4& transform = m_modelTransforms[i];

        for (const QVector3D& v : model->vertices()) {
            QVector4D transformed = transform * QVector4D(v, 1.0f);
            QVector3D tv = transformed.toVector3DAffine();

            minBound.setX(std::min(minBound.x(), tv.x()));
            minBound.setY(std::min(minBound.y(), tv.y()));
            minBound.setZ(std::min(minBound.z(), tv.z()));

            maxBound.setX(std::max(maxBound.x(), tv.x()));
            maxBound.setY(std::max(maxBound.y(), tv.y()));
            maxBound.setZ(std::max(maxBound.z(), tv.z()));
        }
    }

    QVector3D center = (minBound + maxBound) / 2.0f;
    QVector3D size = maxBound - minBound;
    float radius = size.length() / 2.0f;

    float fovY = 45.0f;
    float zDistance = radius / std::tan(qDegreesToRadians(fovY / 2.0f)) + radius;

    QVector3D cameraPos(center.x(), center.y(), center.z() + zDistance);

    m_camera->setPosition(cameraPos);
    m_camera->setTarget(center);
    m_camera->setUp(QVector3D(0.0f, 1.0f, 0.0f));

    update();
}
}

#include "viewport.h"
#include "Viewport/logger.h"

namespace Viewer{

ViewportWidget::ViewportWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat :: CoreProfile);
    setFormat(format);
    m_camera = std::make_unique<CameraPerspective>();
}

bool ViewportWidget::loadTextureForModel(int modelIndex, const QString &texturePath)
{
    if (modelIndex < 0 || modelIndex >= m_modelsGL.size())
        return false;

    ModelGL* modelGL = dynamic_cast<ModelGL*>(m_modelsGL[modelIndex]);
    if (!modelGL)
        return false;

    return m_renderer.loadTexture(*modelGL, texturePath);
}

bool ViewportWidget::loadTextureForModel(int modelIndex, const QImage &image)
{
    if (modelIndex < 0 || modelIndex >= m_modelsGL.size())
        return false;

    ModelGL* modelGL = dynamic_cast<ModelGL*>(m_modelsGL[modelIndex]);
    if (!modelGL)
        return false;

    return m_renderer.loadTextureFromImage(*modelGL, image);
}

bool ViewportWidget::loadTextureForModel(int modelIndex, const unsigned char *data, int width, int height, GLenum format)
{
    if (modelIndex < 0 || modelIndex >= m_modelsGL.size())
        return false;

    ModelGL* modelGL = dynamic_cast<ModelGL*>(m_modelsGL[modelIndex]);
    if (!modelGL)
        return false;

    return m_renderer.loadTextureFromData(*modelGL, data, width, height, format);
}

void ViewportWidget::resizeEvent(QResizeEvent *event)
{
    m_viewportWidth = event->size().width();
    m_viewportHeight = event->size().height();
    m_camera->setViewportSize(m_viewportWidth, m_viewportHeight);
    QOpenGLWidget::resizeEvent(event);
}

void ViewportWidget::initializeGL()
{
    m_renderer.initialize();
    m_camera->setViewportSize(width(), height());
}

void ViewportWidget::paintGL()
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = m_camera->viewMatrix();
    QMatrix4x4 proj = m_camera->projectionMatrix();

    for (int i = 0; i < m_modelsGL.size(); ++i) {
        if (!m_modelsGL[i] || !m_modelsGL[i]->isValid())
            continue;
        QMatrix4x4 mvp = proj * view * m_modelTransforms[i];
        m_renderer.render(*m_modelsGL[i], mvp);
    }
}

void ViewportWidget::wheelEvent(QWheelEvent *event)
{
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

void ViewportWidget::switchToPerspective()
{
    if (m_camera->type() == CameraType::Perspective)
        return;
    auto newCam = std::make_unique<CameraPerspective>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);
    update();
}

void ViewportWidget::switchToOrthographic()
{
    if (m_camera->type() == CameraType::Orthographic)
        return;
    auto newCam = std::make_unique<CameraOrthographic>();
    newCam->setViewportSize(m_viewportWidth, m_viewportHeight);
    m_camera = std::move(newCam);
    update();
}

void ViewportWidget::fitToView()
{
    if (m_modelsGL.isEmpty())
        return;
    if (m_modelTransforms.isEmpty())
        return;

    QVector3D minBound(std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max(),
                       std::numeric_limits<float>::max());

    QVector3D maxBound(-std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max());

    for (int i = 0; i < m_modelsGL.size(); ++i) {
        const ModelGL* model = m_modelsGL[i];
        const QMatrix4x4& transform = m_modelTransforms[i];

        for (const QVector3D& v : model->getModelData()->vertices()) {
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

void ViewportWidget::setModels(const QVector<ObjectGL*>& models, const QVector<QMatrix4x4>& transforms)
{
    m_modelsGL.clear();
    for (ObjectGL* obj : models) {
        ModelGL* modelGL = dynamic_cast<ModelGL*>(obj);
        if (modelGL)
            m_modelsGL.append(modelGL);
    }
    m_modelTransforms = transforms;
    update();
}

}

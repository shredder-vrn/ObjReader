#include "cameraort.h"
#include "Viewport/logger.h"

namespace Viewer{

CameraOrthographic::CameraOrthographic()
{
    LogDebug("CameraPer::CameraPer - запустили конструктор");
    updateViewMatrix();
    updateProjectionMatrix();
    LogDebug("CameraPer::CameraPer - запустили конструктор");
}

QMatrix4x4 CameraOrthographic::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 CameraOrthographic::projectionMatrix() const
{
    return m_projectionMatrix;
}

void CameraOrthographic::setViewportSize(const int width, const int height)
{
    m_width = width;
    m_height = height;
    updateProjectionMatrix();
}

void CameraOrthographic::rotateAroundTarget(const float deltaX, const float deltaY)
{
    const float sensitivity = 0.5f;

    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), -deltaX * sensitivity);
    m_position = m_target + yawRotation.rotatedVector(m_position - m_target);

    QVector3D horizontalAxis = QVector3D::crossProduct(m_position - m_target, QVector3D(0.0f, 1.0f, 0.0f)).normalized();
    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(horizontalAxis, -deltaY * sensitivity);
    m_position = m_target + pitchRotation.rotatedVector(m_position - m_target);

    updateViewMatrix();
}

void CameraOrthographic::zoom(const float delta)
{
    const float zoomSpeed = 0.1f;
    m_zoomFactor -= delta * zoomSpeed;
    if (m_zoomFactor < 0.1f) m_zoomFactor = 0.1f;

    updateProjectionMatrix();
}

void CameraOrthographic::setPosition(const QVector3D &pos)
{
    m_position = pos;
    updateViewMatrix();
}

void CameraOrthographic::updateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position, m_target, m_up);
}

void CameraOrthographic::updateProjectionMatrix()
{
    float halfWidth = (m_right - m_left) / 2.0f * m_zoomFactor;
    float halfHeight = (m_top - m_bottom) / 2.0f * m_zoomFactor;

    float left = -halfWidth;
    float right = halfWidth;
    float bottom = -halfHeight;
    float top = halfHeight;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.ortho(left, right, bottom, top, m_nearPlane, m_farPlane);
}
}

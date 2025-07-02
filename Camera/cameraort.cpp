#include "cameraort.h"

namespace Viewer{

CameraOrt::CameraOrt()
{
    updateViewMatrix();
    updateProjectionMatrix();
}

QMatrix4x4 CameraOrt::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 CameraOrt::projectionMatrix() const
{
    return m_projectionMatrix;
}

void CameraOrt::setViewportSize(const int width, const int height)
{
    m_width = width;
    m_height = height;
    updateProjectionMatrix();
}

void CameraOrt::rotateAroundTarget(const float deltaX, const float deltaY)
{
    const float sensitivity = 0.5f;

    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 1.0f, 0.0f), -deltaX * sensitivity);
    m_position = m_target + yawRotation.rotatedVector(m_position - m_target);

    QVector3D horizontalAxis = QVector3D::crossProduct(m_position - m_target, QVector3D(0.0f, 1.0f, 0.0f)).normalized();
    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(horizontalAxis, -deltaY * sensitivity);
    m_position = m_target + pitchRotation.rotatedVector(m_position - m_target);

    updateViewMatrix();
}

void CameraOrt::zoom(const float delta)
{
    const float zoomSpeed = 0.1f;
    m_zoomFactor -= delta * zoomSpeed;
    if (m_zoomFactor < 0.1f) m_zoomFactor = 0.1f;

    updateProjectionMatrix();
}

void CameraOrt::updateViewMatrix()
{
    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position, m_target, m_up);
}

void CameraOrt::updateProjectionMatrix()
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

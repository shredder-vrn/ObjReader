#include "cameraorthographic.h"

namespace Viewer{

CameraOrthographic::CameraOrthographic()
{
    updateViewMatrix();
    updateProjectionMatrix();
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

QVector3D CameraOrthographic::position() const { return m_position; }

QVector3D CameraOrthographic::target() const { return m_target; }

QVector3D CameraOrthographic::up() const { return m_up; }

void CameraOrthographic::setPosition(const QVector3D &pos)
{
    m_position = pos;
    updateViewMatrix();
}

void CameraOrthographic::setTarget(const QVector3D &target) { m_target = target; }

void CameraOrthographic::setUp(const QVector3D &up) { m_up = up; }

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

#include "camera.h"

#include <cmath>
#include <QtMath>

namespace Viewer {
Camera :: Camera()
{
    updateViewMatrix();
}

void Camera :: setViewportSize(const int width, const int height)
{
    this->m_cameraWidth = width;
    this->m_cameraHeight = height;

    m_cameraProjectionMatrix.setToIdentity();
    m_cameraProjectionMatrix.perspective(m_cameraFov, width / (float)height, m_cameraNearPlane, m_cameraFarPlane);
}

QMatrix4x4 Camera :: viewMatrix() const
{
    return m_cameraViewMatrix;
}

QMatrix4x4 Camera :: projectionMatrix() const
{
    return m_cameraProjectionMatrix;
}

void Camera :: updateViewMatrix()
{
    QVector3D rotatedFront = m_cameraOrientation.rotatedVector(m_cameraFront);
    QVector3D rotatedUp = m_cameraOrientation.rotatedVector(m_cameraUp);

    QVector3D cameraPosition = m_cameraTarget - rotatedFront * m_distanceToTarget;

    m_cameraViewMatrix.setToIdentity();
    m_cameraViewMatrix.lookAt(cameraPosition, m_cameraTarget, rotatedUp);
}

void Camera::rotateAroundTarget(const float deltaX, const float deltaY)
{
    const float rotationSpeed = 0.5f;

    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), deltaX * rotationSpeed);
    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -deltaY * rotationSpeed);

    m_cameraOrientation = m_cameraOrientation * pitchRotation * yawRotation;
    m_cameraOrientation.normalize();

    updateViewMatrix();
}

void Camera::zoom(const float delta) {
    float zoomSpeed = 3.0f;

    QVector3D direction = (m_cameraTarget - m_cameraPosition).normalized();

    m_distanceToTarget = qMax(1.0f, m_distanceToTarget - delta * zoomSpeed);
    m_cameraPosition += direction * delta * zoomSpeed;

    updateViewMatrix();
}
}


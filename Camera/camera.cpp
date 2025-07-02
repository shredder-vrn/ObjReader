#include "camera.h"

#include <cmath>
#include <QtMath>

namespace Viewer {

Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";
    qDebug() << "cameraPosition" << m_cameraPosition;
    qDebug() << "cameraUp" << m_cameraUp;
    qDebug() << "cameraTarget" << m_cameraTarget;
    qDebug() << "cameraWidth" << m_cameraWidth;
    qDebug() << "cameraHeight" << m_cameraHeight;
    qDebug() << "cameraFov" << m_cameraFov;
    qDebug() << "cameraNearPlane" << m_cameraNearPlane;
    qDebug() << "cameraFarPlane" << m_cameraFarPlane;

    updateViewMatrix();
}


void Camera :: setViewportSize(const int width, const int height)
{
    qDebug() << "Camera :: setViewportSize : запустили метод setViewportSize";

    this->m_cameraWidth = width;
    this->m_cameraHeight = height;

    m_cameraProjectionMatrix.setToIdentity();
    m_cameraProjectionMatrix.perspective(m_cameraFov, width / (float)height, m_cameraNearPlane, m_cameraFarPlane);
}

QMatrix4x4 Camera :: viewMatrix() const
{
    qDebug() << "Camera :: getViewMatrix : запустили метод getViewMatrix";
    return m_cameraViewMatrix;
}

QMatrix4x4 Camera :: projectionMatrix() const
{
    qDebug() << "Camera :: getProjectionMatrix : запустили метод getProjectionMatrix";
    return m_cameraProjectionMatrix;
}

void Camera :: updateViewMatrix()
{
    qDebug() << "Camera :: updateViewMatrix : запустили метод updateViewMatrix";
    QVector3D rotatedFront = m_cameraOrientation.rotatedVector(m_cameraFront);
    QVector3D rotatedUp = m_cameraOrientation.rotatedVector(m_cameraUp);

    QVector3D cameraPosition = m_cameraTarget - rotatedFront * m_distanceToTarget;

    m_cameraViewMatrix.setToIdentity();
    m_cameraViewMatrix.lookAt(cameraPosition, m_cameraTarget, rotatedUp);
}

void Camera::rotateAroundTarget(const float deltaX, const float deltaY)
{
    qDebug() << "Camera :: rotateAroundTarget : запустили метод rotateAroundTarget";
    const float rotationSpeed = 0.5f;

    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), deltaX * rotationSpeed);
    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -deltaY * rotationSpeed);

    m_cameraOrientation = m_cameraOrientation * pitchRotation * yawRotation;
    m_cameraOrientation.normalize();

    updateViewMatrix();
}

void Camera::zoom(const float delta) {
    qDebug() << "Camera :: zoom : запустили метод zoom";
    float zoomSpeed = 3.0f;

    QVector3D direction = (m_cameraTarget - m_cameraPosition).normalized();

    m_distanceToTarget = qMax(1.0f, m_distanceToTarget - delta * zoomSpeed);
    m_cameraPosition += direction * delta * zoomSpeed;

    updateViewMatrix();
}
}


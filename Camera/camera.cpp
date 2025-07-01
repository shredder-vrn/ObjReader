#include "camera.h"

#include <cmath>
#include <QtMath>


Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";
    qDebug() << "cameraPosition" << cameraPosition;
    qDebug() << "cameraUp" << cameraUp;
    qDebug() << "cameraTarget" << cameraTarget;
    qDebug() << "cameraWidth" << cameraWidth;
    qDebug() << "cameraHeight" << cameraHeight;
    qDebug() << "cameraFov" << cameraFov;
    qDebug() << "cameraNearPlane" << cameraNearPlane;
    qDebug() << "cameraFarPlane" << cameraFarPlane;

    updateViewMatrix();
}


void Camera :: setViewportSize(int width, int height)
{
    qDebug() << "Camera :: setViewportSize : запустили метод setViewportSize";

    this->cameraWidth = width;
    this->cameraHeight = height;

    cameraProjectionMatrix.setToIdentity();
    cameraProjectionMatrix.perspective(cameraFov, width / (float)height, cameraNearPlane, cameraFarPlane);
}

QMatrix4x4 Camera :: getViewMatrix() const
{
    qDebug() << "Camera :: getViewMatrix : запустили метод getViewMatrix";
    return cameraViewMatrix;
}

QMatrix4x4 Camera :: getProjectionMatrix() const
{
    qDebug() << "Camera :: getProjectionMatrix : запустили метод getProjectionMatrix";
    return cameraProjectionMatrix;
}

void Camera :: updateViewMatrix()
{
    qDebug() << "Camera :: updateViewMatrix : запустили метод updateViewMatrix";

    QVector3D rotatedFront = cameraOrientation.rotatedVector(cameraFront);
    QVector3D rotatedUp = cameraOrientation.rotatedVector(cameraUp);

    QVector3D cameraPosition = cameraTarget - rotatedFront * distanceToTarget;

    cameraViewMatrix.setToIdentity();
    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, rotatedUp);
}

void Camera::rotateAroundTarget(float deltaX, float deltaY)
{
    qDebug() << "Camera :: rotateAroundTarget : запустили метод rotateAroundTarget";
    const float rotationSpeed = 0.5f;

    QQuaternion yawRotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), deltaX * rotationSpeed);
    QQuaternion pitchRotation = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -deltaY * rotationSpeed);

    cameraOrientation = cameraOrientation * pitchRotation * yawRotation;
    cameraOrientation.normalize();

    updateViewMatrix();
}

void Camera::zoom(float delta) {
    qDebug() << "Camera :: zoom : запустили метод zoom";
    float zoomSpeed = 1.0f;

    QVector3D direction = (cameraTarget - cameraPosition).normalized();

    distanceToTarget = qMax(1.0f, distanceToTarget - delta * zoomSpeed);
    cameraPosition += direction * delta * zoomSpeed;

    updateViewMatrix();
}

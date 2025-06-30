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

    cameraViewMatrix.setToIdentity();
    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
}

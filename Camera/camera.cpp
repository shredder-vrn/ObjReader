#include "camera.h"

#include <cmath>
#include <QtMath>


Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";

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

    QMatrix4x4 result;

    result.setToIdentity();
    result.setRow(3, {0, 0,0,1});

    return result;
    //return cameraViewMatrix;
}

QMatrix4x4 Camera :: getProjectionMatrix() const
{
    qDebug() << "Camera :: getProjectionMatrix : запустили метод getProjectionMatrix";

    QMatrix4x4 result;

    result.setToIdentity();

    return result;
    // return cameraProjectionMatrix;
}

void Camera :: updateViewMatrix()
{
    qDebug() << "Camera :: updateViewMatrix : запустили метод updateViewMatrix";

    cameraViewMatrix.setToIdentity();
    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);
}

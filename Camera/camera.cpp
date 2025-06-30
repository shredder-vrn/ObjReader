#include "camera.h"

#include <cmath>
#include <QtMath>


Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";

    qDebug() << "cameraPosition" << cameraPosition;
    qDebug() << "cameraUp" << cameraUp;
    qDebug() << "cameraTarget" << cameraTarget;
    qDebug() << "cameraViewMatrix" << cameraViewMatrix;
    qDebug() << "cameraProjectionMatrix" << cameraProjectionMatrix;
    qDebug() << "cameraWidth" << cameraWidth;
    qDebug() << "cameraHeight" << cameraHeight;
    qDebug() << "cameraFov" << cameraFov;
    qDebug() << "cameraNearPlane" << cameraNearPlane;
    qDebug() << "cameraFarPlane" << cameraFarPlane;

    updateViewMatrix();

    qDebug() << "Camera :: Camera : конструктор отработал, все преобразования обработаны.";
}


void Camera :: setViewportSize(int width, int height)
{
    qDebug() << "Camera :: setViewportSize : запустили метод setViewportSize";

    qDebug() << "width" << width;
    qDebug() << "height" << height;

    this->cameraWidth = width;
    this->cameraHeight = height;

    qDebug() << "cameraWidth" << cameraWidth;
    qDebug() << "cameraHeight" << cameraHeight;

    cameraProjectionMatrix.setToIdentity();
    qDebug() << "cameraProjectionMatrix - setToIdentity():" << cameraProjectionMatrix;

    qDebug() << "cameraProjectionMatrix - 'до' 'cameraProjectionMatrix()':" << cameraProjectionMatrix;
    cameraProjectionMatrix.perspective(cameraFov, width / (float)height, cameraNearPlane, cameraFarPlane);
    qDebug() << "cameraProjectionMatrix - 'после' 'cameraProjectionMatrix()':" << cameraProjectionMatrix;
    qDebug() << "Camera :: setViewportSize : метод setViewportSize отработал";

}

QMatrix4x4 Camera :: getViewMatrix() const
{
    qDebug() << "Camera :: getViewMatrix : запустили метод getViewMatrix";

    QMatrix4x4 result;
    result.setToIdentity();
    result = result * 0.5;
    result.setRow(3, {0, 0,0,1});

    return cameraViewMatrix;

    qDebug() << "Camera :: getViewMatrix : метод getViewMatrix отработал";
}

QMatrix4x4 Camera :: getProjectionMatrix() const
{
    qDebug() << "Camera :: getProjectionMatrix : запустили метод getProjectionMatrix";
    qDebug() << "cameraProjectionMatrix" << cameraProjectionMatrix;
    QMatrix4x4 result;

    result.setToIdentity();

    return result;

    // return cameraProjectionMatrix;

    qDebug() << "Camera :: getProjectionMatrix : метод getProjectionMatrix отработал";
}

void Camera :: updateViewMatrix()
{
    qDebug() << "Camera :: updateViewMatrix : запустили метод updateViewMatrix";

    cameraViewMatrix.setToIdentity();

    qDebug() << "cameraViewMatrix" <<cameraViewMatrix;

    qDebug() << "начало работы метода lookAt: " << "cameraPosition: " <<cameraPosition << ", cameraTarget: " <<cameraTarget << " cameraUp: " <<cameraUp;

    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);

    qDebug() << "метод lookAt отработал: " <<cameraViewMatrix;

    qDebug() << "Camera :: updateViewMatrix : метод updateViewMatrix отработал";
}

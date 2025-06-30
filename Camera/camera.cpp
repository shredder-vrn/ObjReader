#include "camera.h"

#include <cmath>
#include <QtMath>


Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";

    qDebug() << "cameraYaw" << cameraYaw;
    qDebug() << "cameraPitch" << cameraPitch;
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

    return cameraViewMatrix;

    qDebug() << "Camera :: getViewMatrix : метод getViewMatrix отработал";
}

QMatrix4x4 Camera :: getProjectionMatrix() const
{
    qDebug() << "Camera :: getProjectionMatrix : запустили метод getProjectionMatrix";
    qDebug() << "cameraProjectionMatrix" << cameraProjectionMatrix;

    return cameraProjectionMatrix;

    qDebug() << "Camera :: getProjectionMatrix : метод getProjectionMatrix отработал";
}

void Camera :: rotate(float dx, float dy)
{
    qDebug() << "Camera :: rotate : запустили метод rotate";

    cameraYaw += dx * 0.1f;
    cameraPitch -= dy * 0.1f;
    cameraPitch = qBound(-89.0f, cameraPitch, 89.0f);

    updateViewMatrix();

    qDebug() << "Camera :: rotate : метод rotate отработал";

}

void Camera :: setPosition(const QVector3D& newPosition)
{
    qDebug() << "Camera :: setPosition : запустили метод setPosition";

    cameraPosition = newPosition;
    updateViewMatrix();

    qDebug() << "Camera :: setPosition : метод setPosition отработал";
}

void Camera :: updateViewMatrix()
{
    qDebug() << "Camera :: updateViewMatrix : запустили метод updateViewMatrix";

    cameraViewMatrix.setToIdentity();

    qDebug() << "cameraViewMatrix" <<cameraViewMatrix;


    QVector3D front;

    front.setX(cos(qDegreesToRadians(cameraYaw)) * cos(qDegreesToRadians(cameraPitch)));
    front.setY(sin(qDegreesToRadians(cameraPitch)));
    front.setZ(sin(qDegreesToRadians(cameraYaw)) * cos(qDegreesToRadians(cameraPitch)));
    front.normalize();

    qDebug() << "начало работы метода lookAt: " << "cameraPosition: " <<cameraPosition << ", cameraTarget: " <<cameraTarget << " cameraUp: " <<cameraUp;

    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);

    qDebug() << "метод lookAt отработал: " <<cameraViewMatrix;

    qDebug() << "Camera :: updateViewMatrix : метод updateViewMatrix отработал";
}

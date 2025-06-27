#include "camera.h"

#include <cmath>
#include <QtMath>

<<<<<<< Updated upstream
Camera::Camera() {
=======
Camera :: Camera()
{
    qDebug() << "Camera :: Camera : запустили конструктор";

    qDebug() << "cameraYaw" << cameraYaw;
    qDebug() << "cameraPitch" << cameraPitch;
    qDebug() << "cameraPosition" << cameraPosition;
    qDebug() << "cameraFront" << cameraFront;
    qDebug() << "cameraUp" << cameraUp;
    qDebug() << "cameraTarget" << cameraTarget;
    qDebug() << "cameraViewMatrix" << cameraViewMatrix;
    qDebug() << "cameraProjectionMatrix" << cameraProjectionMatrix;
    qDebug() << "cameraWidth" << cameraWidth;
    qDebug() << "cameraHeight" << cameraHeight;
    qDebug() << "cameraFov" << cameraFov;
    qDebug() << "cameraNearPlane" << cameraNearPlane;
    qDebug() << "cameraFarPlane" << cameraFarPlane;

>>>>>>> Stashed changes
    updateViewMatrix();

    qDebug() << "Camera :: Camera : конструктор отработал";
}

<<<<<<< Updated upstream
void Camera::setViewportSize(int width, int height) {
    this->width = width;
    this->height = height;
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(fov, width / (float)height, nearPlane, farPlane);
}

QMatrix4x4 Camera::getViewMatrix() const {
    return viewMatrix;
}

QMatrix4x4 Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::rotate(float dx, float dy) {
    yaw += dx * 0.1f;
    pitch -= dy * 0.1f;
    pitch = qBound(-89.0f, pitch, 89.0f);
=======
void Camera :: setViewportSize(int width, int height)
{
    qDebug() << "Camera :: setViewportSize : запустили метод setViewportSize";

    this->cameraWidth = width;
    this->cameraHeight = height;

    cameraProjectionMatrix.setToIdentity();
    cameraProjectionMatrix.perspective(cameraFov, width / (float)height, cameraNearPlane, cameraFarPlane);

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
    return cameraProjectionMatrix;
    qDebug() << "Camera :: getProjectionMatrix : метод getProjectionMatrix отработал";

}

void Camera :: rotate(float dx, float dy)
{
    qDebug() << "Camera :: rotate : запустили метод rotate";
>>>>>>> Stashed changes

    cameraYaw += dx * 0.1f;
    cameraPitch -= dy * 0.1f;
    cameraPitch = qBound(-89.0f, cameraPitch, 89.0f);

    cameraFront.setX(cos(qDegreesToRadians(cameraYaw)) * cos(qDegreesToRadians(cameraPitch)));
    cameraFront.setY(sin(qDegreesToRadians(cameraPitch)));
    cameraFront.setZ(sin(qDegreesToRadians(cameraYaw)) * cos(qDegreesToRadians(cameraPitch)));
    cameraFront.normalize();

    updateViewMatrix();
<<<<<<< Updated upstream
}

void Camera::setPosition(const QVector3D& newPosition) {
    position = newPosition;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    QVector3D target = position + front;
    viewMatrix.lookAt(position, target, up);
=======
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

    qDebug() << "viewMatrix" <<cameraViewMatrix;

    qDebug() << "начало работы метода lookAt: " <<cameraPosition << cameraTarget << cameraUp;

    cameraViewMatrix.lookAt(cameraPosition, cameraTarget, cameraUp);

    qDebug() << "метод lookAt отработал: " <<cameraViewMatrix;

    qDebug() << "Camera :: updateViewMatrix : метод updateViewMatrix отработал";
>>>>>>> Stashed changes
}

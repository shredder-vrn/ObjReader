#include "camera.h"

#include <cmath>
#include <QtMath>

Camera::Camera() {
    updateViewMatrix();
}

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

    front.setX(cos(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.setY(sin(qDegreesToRadians(pitch)));
    front.setZ(sin(qDegreesToRadians(yaw)) * cos(qDegreesToRadians(pitch)));
    front.normalize();

    updateViewMatrix();
}

void Camera::setPosition(const QVector3D& newPosition) {
    position = newPosition;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
    QVector3D target = position + front;
    viewMatrix.lookAt(position, target, up);
}

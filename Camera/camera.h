#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
    Camera();

    void setViewportSize(int width, int height);
    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;

    void rotate(float dx, float dy);
    void setPosition(const QVector3D& position);

private:
    void updateViewMatrix();

    QVector3D position = QVector3D(0.0f, 0.0f, -7.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;

    QVector3D front = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);

    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    int width = 800;
    int height = 600;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
};

#endif // CAMERA_H

#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera {
public:
    Camera();

    void setViewportSize(int width, int height);
    void zoom(float delta);

    QMatrix4x4 getViewMatrix() const;
    QMatrix4x4 getProjectionMatrix() const;

private:
    void updateViewMatrix();

    QVector3D cameraPosition = QVector3D(0.0f, 0.0f, 20.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
    QMatrix4x4 cameraViewMatrix;
    QMatrix4x4 cameraProjectionMatrix;

    int cameraWidth = 800;
    int cameraHeight = 600;
    float cameraFov = 45.0f;
    float cameraNearPlane = 0.1f;
    float cameraFarPlane = 100.0f;
};

#endif // CAMERA_H

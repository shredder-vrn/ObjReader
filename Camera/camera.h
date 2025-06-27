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

<<<<<<< Updated upstream
    QVector3D position = QVector3D(0.0f, 0.0f, -7.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;

    QVector3D front = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D up = QVector3D(0.0f, 1.0f, 0.0f);
=======
    float cameraYaw = -90.0f;
    float cameraPitch = 0.0f;

    QVector3D cameraPosition = QVector3D(2.0f, 3.0f, 5.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
>>>>>>> Stashed changes

    QMatrix4x4 cameraViewMatrix;
    QMatrix4x4 cameraProjectionMatrix;

    int cameraWidth = 800;
    int cameraHeight = 600;
    float cameraFov = 45.0f;
    float cameraNearPlane = 0.1f;
    float cameraFarPlane = 100.0f;
};

#endif // CAMERA_H

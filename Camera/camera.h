#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>

namespace Viewer {

class Camera
{
public:
    Camera();

    void setViewportSize(const int width, const int height);
    void zoom(const float delta);
    void rotateAroundTarget(const float deltaX, const float deltaY);

    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionMatrix() const;

private:
    void updateViewMatrix();

    QVector3D m_cameraPosition = QVector3D(0.0f, 0.0f, 20.0f);
    QVector3D m_cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D m_cameraTarget = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D m_cameraFront = QVector3D(0, 0, -1);

    QMatrix4x4 m_cameraViewMatrix;
    QMatrix4x4 m_cameraProjectionMatrix;

    QQuaternion m_cameraOrientation = QQuaternion::fromAxisAndAngle(QVector3D{0, 1, 0}, 1);

    int m_cameraWidth = 800;
    int m_cameraHeight = 600;
    float m_cameraFov = 45.0f;
    float m_cameraNearPlane = 0.1f;
    float m_cameraFarPlane = 100.0f;

    float m_distanceToTarget = 20.0f;
    float m_cameraYaw = 0.0f;
    float m_cameraPitch = 0.0f;
};

}

#endif // CAMERA_H

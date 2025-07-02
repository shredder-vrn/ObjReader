#ifndef CAMERAPER_H
#define CAMERAPER_H

#include "Camera/сameraInterface.h"

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector>
#include <QVector3D>

class CameraPer : public CameraInterface
{
public:
    CameraPer();
    ~CameraPer() override {};

    QMatrix4x4 viewMatrix() const override;
    QMatrix4x4 projectionMatrix() const override;

    void setViewportSize(const int width, const int height) override;
    void rotateAroundTarget(const float deltaX, const float deltaY) override;
    void zoom(const float delta) override;

    CameraType type() const override { return CameraType::Perspective; }
private:
    void updateViewMatrix();

    QVector3D m_position = QVector3D(0.0f, 0.0f, 20.0f);
    QVector3D m_up = QVector3D(0.0f, 1.0f, 0.0f);
    QVector3D m_target = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D m_front = QVector3D(0.0f, 0.0f, -1.0f);

    int m_width = 800;
    int m_height = 600;
    float m_fov = 45.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 100.0f;
    float m_distance = 20.0f;
    float m_cameraYaw = 0.0f;
    float m_cameraPitch = 0.0f;

    QQuaternion m_orientation = QQuaternion::fromAxisAndAngle(QVector3D{0, 1, 0}, 0);

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
};

#endif // CAMERAPER_H

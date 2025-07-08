#ifndef VIEWER_CAMERAPERSPECTIVE_H
#define VIEWER_CAMERAPERSPECTIVE_H

#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector>
#include <QVector3D>

#include "Camera/camera.h"

namespace Viewer{

class CameraPerspective : public Camera
{
public:
    CameraPerspective();
    virtual ~CameraPerspective() override {};

    QMatrix4x4 viewMatrix() const override;
    QMatrix4x4 projectionMatrix() const override;

    void setViewportSize(const int width, const int height) override;
    void rotateAroundTarget(const float deltaX, const float deltaY) override;
    void zoom(const float delta) override;

    QVector3D position() const override { return m_position; }
    QVector3D target() const override { return m_target; }
    QVector3D up() const override { return m_up; }

    void setPosition(const QVector3D& pos) override {
        m_position = pos;
        updateViewMatrix();
    }
    void setTarget(const QVector3D& target) override { m_target = target; }
    void setUp(const QVector3D& up) override { m_up = up; }


    CameraType type() const override { return CameraType::Perspective; }
private:
    void updateViewMatrix();

    QVector3D m_position = QVector3D(0.0f, 0.0f, 10.0f);
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
}
#endif // VIEWER_CAMERAPERSPECTIVE_H

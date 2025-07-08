#ifndef CAMERAORT_H
#define CAMERAORT_H

#include <QMatrix4x4>
#include <QVector3D>

#include "Camera/сamerainterface.h"

namespace Viewer {

class CameraOrthographic : public CameraInterface
{
public:
    CameraOrthographic();
    ~CameraOrthographic() override = default;

    QMatrix4x4 viewMatrix() const override;
    QMatrix4x4 projectionMatrix() const override;

    //! REVIEW: setAspectRatio
    void setViewportSize(const int width, const int height) override;
    void rotateAroundTarget(const float deltaX, const float deltaY) override;
    void zoom(const float delta) override;

    QVector3D position() const override { return m_position; }
    QVector3D target() const override { return m_target; }
    QVector3D up() const override { return m_up; }

    void setPosition(const QVector3D& pos) override;
    void setTarget(const QVector3D& target) override { m_target = target; }
    void setUp(const QVector3D& up) override { m_up = up; }

    CameraType type() const override { return CameraType::Orthographic; }

private:
    void updateViewMatrix();
    void updateProjectionMatrix();

    QVector3D m_position = QVector3D(0.0f, 0.0f, 10.0f);
    QVector3D m_target = QVector3D(0.0f, 0.0f, 0.0f);
    QVector3D m_up = QVector3D(0.0f, 1.0f, 0.0f);

    float m_left = -10.0f;
    float m_right = 10.0f;
    float m_bottom = -10.0f;
    float m_top = 10.0f;
    //! REVIEW: 0.01
    float m_nearPlane = -100.0f;
    float m_farPlane = 100.0f;

    float m_zoomFactor = 1.0f;

    int m_width = 800;
    int m_height = 600;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
};
}
#endif // CAMERAORT_H

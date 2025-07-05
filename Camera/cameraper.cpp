#include "cameraper.h"
namespace Viewer{
CameraPer::CameraPer()
{
    updateViewMatrix();
}

QMatrix4x4 CameraPer::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 CameraPer::projectionMatrix() const
{
    return m_projectionMatrix;
}

void CameraPer::setViewportSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, width / (float)height, m_nearPlane, m_farPlane);
}

void CameraPer::zoom(const float delta)
{
    float speed = 1.0f;
    QVector3D direction = (m_target - m_position).normalized();

    m_distance = qMax(1.0f, m_distance - delta * speed);
    m_position += direction * delta * speed;

    updateViewMatrix();
}

void CameraPer::rotateAroundTarget(const float deltaX, const float deltaY)
{
    const float speed = 0.5f;

    QQuaternion yaw = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), deltaX * speed);
    QQuaternion pitch = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -deltaY * speed);

    m_orientation = m_orientation * pitch * yaw;
    m_orientation.normalize();

    updateViewMatrix();
}

void CameraPer::updateViewMatrix()
{
    QVector3D rotatedFront = m_orientation.rotatedVector(m_front);
    QVector3D rotatedUp = m_orientation.rotatedVector(m_up);

    QVector3D cameraPos = m_target - rotatedFront * m_distance;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(cameraPos, m_target, rotatedUp);
}
}

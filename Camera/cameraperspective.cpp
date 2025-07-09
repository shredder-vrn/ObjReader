#include "cameraperspective.h"

namespace Viewer{

CameraPerspective::CameraPerspective()
{
    updateViewMatrix();
    updateOrientationFromPosition();
}

QMatrix4x4 CameraPerspective::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 CameraPerspective::projectionMatrix() const
{
    return m_projectionMatrix;
}

void CameraPerspective::setViewportSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, width / (float)height, m_nearPlane, m_farPlane);
}

void CameraPerspective::zoom(const float delta) {
    m_distance = qMax(1.0f, m_distance * (1.0f - delta * 0.1f));
    updateViewMatrix();
}

QVector3D CameraPerspective::position() const {
    return m_position;
}

QVector3D CameraPerspective::target() const {
    return m_target;
}

QVector3D CameraPerspective::up() const {
    return m_up;
}

void CameraPerspective::setPosition(const QVector3D &pos) {
    m_position = pos;
    m_distance = (m_position - m_target).length();
    updateViewMatrix();
    updateOrientationFromPosition();
}

void CameraPerspective::setTarget(const QVector3D &target) {
    m_target = target;
}

void CameraPerspective::setUp(const QVector3D &up) {
    m_up = up;
}

float CameraPerspective::distance() const {
    return m_distance;
}

void CameraPerspective::setDistance(float distance) {
    m_distance = qMax(0.1f, distance);
    updateViewMatrix();
}

CameraType CameraPerspective::type() const {
    return CameraType::Perspective;
}

void CameraPerspective::rotateAroundTarget(const float deltaX, const float deltaY)
{
    const float speed = 0.5f;

    QQuaternion yaw = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), deltaX * speed);
    QQuaternion pitch = QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), -deltaY * speed);

    m_orientation = m_orientation * pitch * yaw;
    m_orientation.normalize();

    updateViewMatrix();
}

void CameraPerspective::updateViewMatrix()
{
    QVector3D rotatedFront = m_orientation.rotatedVector(m_front);
    QVector3D rotatedUp = m_orientation.rotatedVector(m_up);

    QVector3D cameraPos = m_target - rotatedFront * m_distance;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(cameraPos, m_target, rotatedUp);
}

void CameraPerspective::updateOrientationFromPosition()
{
    QVector3D dir = (m_position - m_target).normalized();

    float yaw = qAtan2(dir.x(), dir.z()) * 180.0f / M_PI;
    float pitch = qAsin(-dir.y()) * 180.0f / M_PI;

    m_orientation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw) *
                     QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pitch);
}

}

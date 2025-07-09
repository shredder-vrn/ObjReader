#include "cameraperspective.h"

namespace Viewer{

CameraPerspective::CameraPerspective()
{
    updateViewMatrix();
}

QMatrix4x4 CameraPerspective::viewMatrix() const
{
    return m_viewMatrix;
}

QQuaternion CameraPerspective::orientation() const {
    return m_orientation;
}

QMatrix4x4 CameraPerspective::projectionMatrix() const
{
    return m_projectionMatrix;
}

QVector3D CameraPerspective::position() const {
    return m_position;
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

void CameraPerspective::setViewportSize(const int width, const int height)
{
    m_width = width;
    m_height = height;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fov, width / (float)height, m_nearPlane, m_farPlane);
}

void CameraPerspective::setOrientation(const QQuaternion &orientation) {
    m_orientation = orientation;
    updateViewMatrix();
}

void CameraPerspective::setTarget(const QVector3D &target) {
    m_target = target;
    updateViewMatrix();
}

void CameraPerspective::setPosition(const QVector3D &pos) {
    m_position = pos;
    m_distance = (m_position - m_target).length();
    updateOrientationFromPosition();
    updateViewMatrix();
}

void CameraPerspective::setUp(const QVector3D &up) {
    m_up = up;
}

QVector3D CameraPerspective::target() const {
    return m_target;
}

CameraType CameraPerspective::type() const {
    return CameraType::Perspective;
}

void CameraPerspective::zoom(const float delta) {
    m_distance = qMax(1.0f, m_distance * (1.0f - delta * 0.1f));
    updateViewMatrix();
}

QVector3D CameraPerspective::up() const {
    return m_up;
}

void CameraPerspective::updateViewMatrix()
{
    QVector3D rotatedFront = m_orientation.rotatedVector(m_front);
    QVector3D rotatedUp = m_orientation.rotatedVector(m_up);

    m_position = m_target - rotatedFront * m_distance;

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(m_position, m_target, rotatedUp);
}

void CameraPerspective::updateOrientationFromPosition()
{
    QVector3D dir = (m_position - m_target).normalized();

    float yaw = qAtan2(dir.x(), dir.z()) * 180.0f / M_PI;
    float pitch = qAtan2(-dir.y(), qSqrt(dir.x() * dir.x() + dir.z() * dir.z())) * 180.0f / M_PI;

    m_orientation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), yaw) *
                     QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), pitch);
}

}
































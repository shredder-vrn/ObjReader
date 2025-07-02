#ifndef СAMERAINTERFACE_H
#define СAMERAINTERFACE_H

#include <QMatrix4x4>
#include <QVector3D>

namespace Viewer {
enum class CameraType {
    Perspective,
    Orthographic
};

class CameraInterface
{
public:
    virtual ~CameraInterface() = default;

    virtual QMatrix4x4 viewMatrix() const = 0;
    virtual QMatrix4x4 projectionMatrix() const = 0;

    virtual void setViewportSize(const int width, const int height) = 0;
    virtual void rotateAroundTarget(const float deltaX, const float deltaY) = 0;
    virtual void zoom(const float delta) = 0;

    virtual CameraType type() const = 0;
    virtual QVector3D position() const = 0;
    virtual QVector3D target() const = 0;
    virtual QVector3D up() const = 0;

    virtual void setPosition(const QVector3D& pos) = 0;
    virtual void setTarget(const QVector3D& target) = 0;
    virtual void setUp(const QVector3D& up) = 0;

};
}
#endif // СAMERAINTERFACE_H

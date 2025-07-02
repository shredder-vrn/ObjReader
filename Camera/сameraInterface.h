#ifndef СAMERAINTERFACE_H
#define СAMERAINTERFACE_H

#include <QMatrix4x4>
#include <QVector3D>

class CameraInterface
{
public:
    virtual ~CameraInterface() = default;

    virtual QMatrix4x4 viewMatrix() const = 0;
    virtual QMatrix4x4 projectionMatrix() const = 0;

    virtual void setViewportSize(const int with, const int height) = 0;
    virtual void rotateAroundTarget(const float deltaX, const float deltaY) = 0;
    virtual void zoom(const float delta) = 0;
};

#endif // СAMERAINTERFACE_H

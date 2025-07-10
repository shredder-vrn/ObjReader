#ifndef VIEWER_OBJECTSGL_H
#define VIEWER_OBJECTSGL_H

#include <QOpenGLFunctions>

namespace Viewer {

class ObjectGL//! REIVIEW: а в чём суть этого интерфейса? можем ли мы нарисовать что-то без точек
{
public:
    virtual ~ObjectGL() = default;

    virtual GLuint vao() const = 0;
    virtual GLuint vertexCount() const = 0;
    virtual GLuint textureId() const = 0;

    virtual void setVao(GLuint vao) = 0;
    virtual void setVertexCount(GLuint count) = 0;

    virtual bool useTexture() const = 0;
    virtual bool useNormals() const = 0;
    virtual bool isValid() const = 0;
    virtual bool hasNormals() const = 0;


    virtual void setTextureId(GLuint id) = 0;
    virtual void setHasTexture(bool enabled) = 0;
    virtual void setUseNormals(bool enabled) = 0;

    virtual void clear() = 0;
    virtual bool operator!=(const ObjectGL &other) const = 0;//! REIVIEW: а это я боюсь спросить зачем?
};

}
#endif // VIEWER_OBJECTSGL_H

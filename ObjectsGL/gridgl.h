#ifndef VIEWER_GRIDGL_H
#define VIEWER_GRIDGL_H

#include <QOpenGLFunctions_3_3_Core>
#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include "ObjectsGL/objectsgl.h"

namespace Viewer{


class GridGL : public ObjectGL, protected QOpenGLFunctions_3_3_Core
{
public:
    GridGL(float size);

    ~GridGL() override;

    GLuint vao() const override;
    GLuint vertexCount() const override;
    GLuint textureId() const override;

    void setVao(GLuint vao) override;
    void setVertexCount(GLuint count) override;

    bool useTexture() const override;
    bool useNormals() const override;
    bool isValid() const override;
    bool hasNormals() const override;

    void setHasTexture(bool enabled) override;
    void setTextureId(GLuint id) override;
    void setUseNormals(bool enabled) override;

    void clear() override;
    bool operator!=(const ObjectGL &other) const override;

private:
    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    int m_size;
    float m_spacing;
};

}

#endif // VIEWER_GRIDGL_H

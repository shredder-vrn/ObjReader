#ifndef VIEWER_MODELGL_H
#define VIEWER_MODELGL_H

#include <QVector>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "Model/modeldata.h"
#include "ObjectsGL/objectsgl.h"

namespace Viewer {


class ModelGL : public ObjectGL
{
public:
    ModelGL();
    ~ModelGL();

    GLuint vao() const override;
    GLuint vertexCount() const override;
    GLuint textureId() const override;

    void setVao(GLuint vao) override;
    void setVertexCount(GLuint count) override;

    bool hasTexture() const override;
    bool useNormals() const override;
    bool isValid() const override;
    bool hasNormals() const override;


    void setHasTexture(bool enabled) override;
    void setUseNormals(bool enabled) override;
    void setTextureId(GLuint id) override;

    void clear() override;
    bool operator!=(const ObjectGL &other) const override;

    void setModelData(const ModelData *modelData);
    const ModelData *getModelData() const;

private:
    ModelData m_modelData;

    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    GLuint m_textureId = 0;

    bool m_hasTexture = false;
    bool m_useNormals = false;
};
}

#endif // VIEWER_MODELGL_H

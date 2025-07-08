#ifndef MODELGL_H
#define MODELGL_H

#include <QVector>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "Model/modeldata.h"

class ModelGL : protected QOpenGLFunctions
{
public:
    ModelGL();
    ~ModelGL();

    void setModelData(const ModelData *modelData);
    const ModelData *getModelData() const;

    GLuint vao() const;
    GLuint vertexCount() const;
    GLuint textureId() const;

    void setVao(GLuint vao);
    void setVertexCount(GLuint count);

    bool hasTexture() const;
    bool useNormals() const;
    bool isValid() const;
    bool hasNormals() const;


    void setHasTexture(bool enabled);
    void setUseNormals(bool enabled);
    void setTextureId(GLuint id);

    void clear();

    bool operator!=(const ModelGL &other) const;

private:
    ModelData m_modelData;

    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    GLuint m_textureId = 0;

    bool m_hasTexture = false;
    bool m_useNormals = false;
};

#endif // MODELGL_H

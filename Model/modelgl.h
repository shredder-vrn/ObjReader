#ifndef MODELGL_H
#define MODELGL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "Model/modeldata.h"

class ModelGL : protected QOpenGLFunctions
{
public:
    ModelGL() = default;
    ~ModelGL() = default;

    void setModelData(const ModelData *modelData);
    const ModelData *getModelData() const {return m_modelData;};

    GLuint vao() const { return m_vao; };
    GLuint vertexCount() const { return m_vertexCount; };
    GLuint textureId() const { return m_textureId; };

    void setVao(GLuint vao) {m_vao = vao;};
    void setVertexCount(GLuint count) {m_vertexCount = count;};

    bool hasTexture() const {return m_hasTexture;};
    bool useNormals() const {return m_useNormals;};
    bool isValid() const;
    bool hasNormals() const;

    void setHasTexture(bool enabled) {m_hasTexture = enabled;};
    void setUseNormals(bool enabled) {m_useNormals = enabled;};
    void setTextureId(GLuint id) {m_textureId = id;};

    void clear();

    bool operator!=(const ModelGL& other) const;

private:
    const ModelData *m_modelData = nullptr;

    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    GLuint m_textureId = 0;

    bool m_hasTexture = false;
    bool m_useNormals = false;
};

#endif // MODELGL_H

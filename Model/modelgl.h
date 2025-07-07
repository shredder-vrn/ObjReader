#ifndef MODELGL_H
#define MODELGL_H

#include <QVector>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

#include "Model/modeldata.h"

//! REVIEW: наследование от QOpenGLFunctions? страшно, поговорим
class ModelGL : protected QOpenGLFunctions
{
public:
    ModelGL() = default;
    ~ModelGL() = default;

    void setModelData(const ModelData *modelData);
    //! REVIEW: этот метод используется? оч страшно
    const ModelData *getModelData() const;

    GLuint vao() const;
    GLuint vertexCount() const;
    GLuint textureId() const;

    //! REVIEW: что такое вао? не оч популярная абревиатура
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

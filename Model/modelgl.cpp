#include "modelgl.h"

void ModelGL::setModelData(const ModelData *modelData)
{
    m_modelData = modelData;
    if (modelData) {
        m_vertexCount = static_cast<GLuint>(modelData->vertices().size());
    }
}

const ModelData *ModelGL::modelData() const
{
    return m_modelData;
}

const QVector<QVector3D> &ModelGL::vertices() const
{
    static QVector<QVector3D> empty;
    return m_modelData ? m_modelData->vertices() : empty;
}

const QVector<QVector2D> &ModelGL::textureVertices() const
{
    static QVector<QVector2D> empty;
    return m_modelData ? m_modelData->textureVertices() : empty;
}

const QVector<QVector3D> &ModelGL::normals() const
{
    static QVector<QVector3D> empty;
    return m_modelData ? m_modelData->normals() : empty;
}

const QVector<int> &ModelGL::faceVertexIndices() const
{
    static QVector<int> empty;
    return m_modelData ? m_modelData->faceVertexIndices() : empty;
}

const QVector<int> &ModelGL::faceTextureVertexIndices() const
{
    static QVector<int> empty;
    return m_modelData ? m_modelData->faceTextureVertexIndices() : empty;
}

const QVector<int> &ModelGL::faceNormalIndices() const
{
    static QVector<int> empty;
    return m_modelData ? m_modelData->faceNormalIndices() : empty;
}

const QVector<int> &ModelGL::polygonStarts() const
{
    static QVector<int> empty;
    return m_modelData ? m_modelData->polygonStarts() : empty;
}

void ModelGL::setVao(GLuint vao) { m_vao = vao; }

void ModelGL::setVertexCount(GLuint count) { m_vertexCount = count; }

void ModelGL::setTextureId(GLuint id) { m_textureId = id; }

bool ModelGL::hasTexture() const
{
}

void ModelGL::setHasTexture(bool enabled) { m_hasTexture = enabled; }

void ModelGL::setUseNormals(bool enabled) { m_useNormals = enabled; }

void ModelGL::clear()
{
}

bool ModelGL::operator!=(const ModelGL &other) const
{
}

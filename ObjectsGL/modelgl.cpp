#include "modelgl.h"
#include "Viewport/logger.h"

namespace Viewer {

ModelGL::ModelGL(){

};

ModelGL::~ModelGL(){

};

void ModelGL::setModelData(const ModelData *modelData)
{
    if (!modelData)
        return;
    m_modelData = *modelData;
}

const ModelData *ModelGL::getModelData() const
{
    return &m_modelData;
}

GLuint ModelGL::vao() const
{
    return m_vao;
}

GLuint ModelGL::vertexCount() const
{
    return m_vertexCount;
}

GLuint ModelGL::textureId() const
{
    return m_textureId;
}

void ModelGL::setVao(GLuint vao)
{
    m_vao = vao;
}

void ModelGL::setVertexCount(GLuint count)
{
    m_vertexCount = count;
}

bool ModelGL::hasTexture() const
{
    return m_hasTexture;
}

bool ModelGL::useNormals() const
{
    return m_useNormals;
}

bool ModelGL::isValid() const
{
    bool valid = !m_modelData.vertices().isEmpty() && !m_modelData.faceVertexIndices().isEmpty();
    return valid;
}

bool ModelGL::hasNormals() const
{
    return !m_modelData.normals().isEmpty();
}

void ModelGL::setHasTexture(bool enabled)
{
    m_hasTexture = enabled;
}

void ModelGL::setUseNormals(bool enabled)
{
    m_useNormals = enabled;
}

void ModelGL::setTextureId(GLuint id)
{
    m_textureId = id;
}

void ModelGL::clear()
{
    m_vao = 0;
    m_vertexCount = 0;
    m_textureId = 0;
    m_hasTexture = false;
    m_useNormals = false;
    m_modelData = ModelData();
}

bool ModelGL::operator!=(const ObjectGL &other) const
{
    bool result = vao() != other.vao() ||
            vertexCount() != other.vertexCount() ||
            textureId() != other.textureId() ||
            hasTexture() != other.hasTexture() ||
            useNormals() != other.useNormals();
    return result;
}

void ModelGL::setWireframeMode(bool enabled)
{
    m_wireframeMode = enabled;
}

bool ModelGL::wireframeMode() const
{
    return m_wireframeMode;
}

QVector3D ModelGL::position() const { return m_position; }

QQuaternion ModelGL::rotation() const { return m_rotation; }

QVector3D ModelGL::scale() const { return m_scale; }

void ModelGL::setPosition(const QVector3D &pos) { m_position = pos; }

void ModelGL::setRotation(const QQuaternion &rot) { m_rotation = rot; }

void ModelGL::setScale(const QVector3D &scale) { m_scale = scale; }


}

#include "modelgl.h"


void ModelGL::setModelData(const ModelData *modelData)
{
    m_modelData = modelData;
    if (modelData) {
        m_vertexCount = static_cast<GLuint>(modelData->vertices().size());
    }
}

const ModelData *ModelGL::getModelData() const
{
    return m_modelData;
}

void ModelGL::setVao(GLuint vao) {
    m_vao = vao;
}

void ModelGL::setVertexCount(GLuint count) {
    m_vertexCount = count;
}

bool ModelGL::hasTexture() const {
    return m_hasTexture;
}

bool ModelGL::useNormals() const {
    return m_useNormals;
}

bool ModelGL::isValid() const {
    return m_vao != 0 && m_vertexCount > 0 && m_modelData && m_modelData->isValid();
}

bool ModelGL::hasNormals() const {
    return m_modelData && !m_modelData->normals().isEmpty();
}

void ModelGL::setHasTexture(bool enabled) {
    m_hasTexture = enabled;
}

void ModelGL::setUseNormals(bool enabled) {
    m_useNormals = enabled;
}

void ModelGL::setTextureId(GLuint id) {
    m_textureId = id;
}

void ModelGL::clear() {
    m_vao = 0;
    m_vertexCount = 0;
    m_textureId = 0;
    m_hasTexture = false;
    m_useNormals = false;
    m_modelData = nullptr;
}

bool ModelGL::operator!=(const ModelGL &other) const {
    return m_vao != other.m_vao ||
           m_vertexCount != other.m_vertexCount ||
           m_textureId != other.m_textureId ||
           m_hasTexture != other.m_hasTexture ||
           m_useNormals != other.m_useNormals;
}

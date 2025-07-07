#include "modelgl.h"

void ModelGL::setModelData(const ModelData *modelData)
{
    m_modelData = modelData;
    if (modelData) {
        m_vertexCount = static_cast<GLuint>(modelData->vertices().size());
    }
}

bool ModelGL::isValid() const {
    return m_vao != 0 && m_vertexCount > 0 && m_modelData && m_modelData->isValid();
}

bool ModelGL::hasNormals() const {
    return m_modelData && !m_modelData->normals().isEmpty();
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

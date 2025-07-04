#include "model.h"

#include <QDebug>


void Model::clear() {
    m_vertices.clear();
    m_textureVertices.clear();
    m_normals.clear();

    m_faceVertexIndices.clear();
    m_faceTextureVertexIndices.clear();
    m_faceNormalIndices.clear();

    m_polygonStarts.clear();
}

bool Model::isValid() const {

    bool valid = !m_vertices.isEmpty() &&
                     !m_faceVertexIndices.isEmpty() &&
                     m_faceVertexIndices.size() % 3 == 0;

    if (!valid) return false;

    if (m_hasTexture && m_textureVertices.isEmpty()) {
        qDebug() << "[WARNING] hasTexture == true, но textureVertices пустые";
        return false;
    }

    if (m_useNormals && m_normals.isEmpty()) {
        qDebug() << "[WARNING] useNormal == true, но normals пустые";
        return false;
    }

    return true;
}

bool Model::operator!=(const Model& other) const {
    return m_faceVertexIndices != other.m_faceVertexIndices ||
           m_vertices != other.m_vertices ||
           m_polygonStarts != other.m_polygonStarts;
}

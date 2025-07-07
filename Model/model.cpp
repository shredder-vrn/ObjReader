#include "model.h"

#include <QDebug>

QVector<QVector3D>& Model::setVertices() {
    return m_vertices;
}

QVector<QVector2D> &Model::setTextureVertices()
{
    return m_textureVertices;
}

QVector<QVector3D> &Model::setNormals()
{
    return m_normals;
}

QVector<int> &Model::setFaceVertexIndices()
{
    return m_faceVertexIndices;
}

QVector<int> &Model::setFaceTextureVertexIndices()
{
    return m_faceTextureVertexIndices;
}

QVector<int> &Model::setFaceNormalIndices()
{
    return m_faceNormalIndices;
}

QVector<int> &Model::setPolygonStarts()
{
    return m_polygonStarts;
}

GLuint Model::vao() const {
    return m_vao;
}

GLuint Model::vertexCount() const {
    return m_vertexCount;
}

GLuint Model::textureId() const {
    return m_textureId;
}

void Model::setVao(GLuint vao) {
    m_vao = vao;
}

void Model::setVertexCount(GLuint count) {
    m_vertexCount = count;
}

bool Model::hasTexture() const {
    return m_hasTexture;
}

bool Model::useNormals() const {
    return m_useNormals;
}

void Model::setHasTexture(bool enabled) {
    m_hasTexture = enabled;
}

void Model::setUseNormals(bool enabled) {
    m_useNormals = enabled;
}

void Model::setTextureId(GLuint id) {
    m_textureId = id;
}

void Model::clear() {
    m_vertices.clear();
    m_textureVertices.clear();
    m_normals.clear();

    m_faceVertexIndices.clear();
    m_faceTextureVertexIndices.clear();
    m_faceNormalIndices.clear();
    m_polygonStarts.clear();

    m_vao = 0;
    m_vertexCount = 0;
    m_textureId = 0;
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
        qDebug() << "[WARNING] useNormals == true, но normals пустые";
        return false;
    }

    return true;
}

bool Model::hasNormals() const {
    return !m_normals.isEmpty();
}

bool Model::operator!=(const Model& other) const {
    return m_faceVertexIndices != other.m_faceVertexIndices ||
           m_vertices != other.m_vertices ||
           m_polygonStarts != other.m_polygonStarts;
}

#include "modeldata.h"
#include "Viewport/logger.h"

namespace Viewer {

ModelData::ModelData()
{
    LogDebug("ModelData::ModelData - запустили конструктор");
    LogDebug("ModelData::ModelData - конструктор отработал");
}

ModelData::~ModelData()
{
    LogDebug("ModelData::ModelData - запустили деструктор");
    LogDebug("ModelData::ModelData - деструктор отработал");
}

ModelData &ModelData::setVertices(const QVector<QVector3D>& vertices)
{
    m_vertices = vertices;
    return *this;
}

ModelData &ModelData::setTextureVertices(const QVector<QVector2D> &v) {
    m_textureVertices = v;
    return *this;
}

ModelData &ModelData::setNormals(const QVector<QVector3D> &n) {
    m_normals = n;
    return *this;
}

ModelData &ModelData::setFaceVertexIndices(const QVector<int> &f) {
    m_faceVertexIndices = f;
    return *this;
}

ModelData &ModelData::setFaceTextureVertexIndices(const QVector<int> &f) {
    m_faceTextureVertexIndices = f;
    return *this;
}

ModelData &ModelData::setFaceNormalIndices(const QVector<int> &f) {
    m_faceNormalIndices = f;
    return *this;
}

ModelData &ModelData::setPolygonStarts(const QVector<int> &p) {
    m_polygonStarts = p;
    return *this;
}

bool ModelData::isValid() const {
    return !m_vertices.isEmpty() && !m_faceVertexIndices.isEmpty() && m_faceVertexIndices.size() % 3 == 0;}

}

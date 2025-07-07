#ifndef MODELDATA_H
#define MODELDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class ModelData
{
public:
    ModelData() = default;
    ~ModelData() = default;

    const QVector<QVector3D> &vertices() const {return m_vertices;};
    const QVector<QVector2D> &textureVertices() const {return m_textureVertices;};
    const QVector<QVector3D> &normals() const {return m_normals;};

    const QVector<int> &faceVertexIndices() const {return m_faceVertexIndices;};
    const QVector<int> &faceTextureVertexIndices() const {return m_faceTextureVertexIndices;};
    const QVector<int> &faceNormalIndices() const {return m_faceNormalIndices;};
    const QVector<int> &polygonStarts() const {return m_polygonStarts;};

    void setVertices(const QVector<QVector3D> &vertices) {m_vertices = vertices;};
    void setTextureVertices(const QVector<QVector2D> &textureVertices) {m_textureVertices = textureVertices;};
    void setNormals(const QVector<QVector3D> &normals) {m_normals = normals;};

    void setFaceVertexIndices(const QVector<int> &faceVertexIndices) {m_faceVertexIndices = faceVertexIndices;};
    void setFaceTextureVertexIndices(const QVector<int> &faceTextureVertexIndices) {m_faceTextureVertexIndices = faceTextureVertexIndices;};
    void setFaceNormalIndices(const QVector<int> &faceNormalIndices) {m_faceNormalIndices = faceNormalIndices;};
    void setPolygonStarts(const QVector<int> &polygonStarts) {m_polygonStarts = polygonStarts;};

    bool isValid() const {
            return !m_vertices.isEmpty() &&
                   !m_faceVertexIndices.isEmpty() &&
                   m_faceVertexIndices.size() % 3 == 0;};

private:
    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_textureVertices;
    QVector<QVector3D> m_normals;

    QVector<int> m_faceVertexIndices;
    QVector<int> m_faceTextureVertexIndices;
    QVector<int> m_faceNormalIndices;
    QVector<int> m_polygonStarts;
};

#endif // MODELDATA_H

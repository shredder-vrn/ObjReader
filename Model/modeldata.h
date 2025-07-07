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

    ModelData &setVertices(const QVector<QVector3D> &v) {
            m_vertices = v;
            return *this;
        }

    ModelData &setTextureVertices(const QVector<QVector2D> &v) {
        m_textureVertices = v;
        return *this;
    }

    ModelData &setNormals(const QVector<QVector3D> &n) {
        m_normals = n;
        return *this;
    }

    ModelData &setFaceVertexIndices(const QVector<int> &f) {
        m_faceVertexIndices = f;
        return *this;
    }

    ModelData &setFaceTextureVertexIndices(const QVector<int> &f) {
        m_faceTextureVertexIndices = f;
        return *this;
    }

    ModelData &setFaceNormalIndices(const QVector<int> &f) {
        m_faceNormalIndices = f;
        return *this;
    }

    ModelData &setPolygonStarts(const QVector<int> &p) {
        m_polygonStarts = p;
        return *this;
    }

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

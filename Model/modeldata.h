#ifndef MODELDATA_H
#define MODELDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

class ModelData
{
public:
    ModelData();
    ~ModelData();

    const QVector<QVector3D> &vertices() const {return m_vertices;}
    const QVector<QVector2D> &textureVertices() const {return m_textureVertices;}
    const QVector<QVector3D> &normals() const {return m_normals;}

    const QVector<int> &faceVertexIndices() const {return m_faceVertexIndices;}
    const QVector<int> &faceTextureVertexIndices() const {return m_faceTextureVertexIndices;}
    const QVector<int> &faceNormalIndices() const {return m_faceNormalIndices;}
    const QVector<int> &polygonStarts() const {return m_polygonStarts;}

    ModelData& setVertices(const QVector<QVector3D>& vertices);

    //! REVIEW: тут явно не хватает ассертов на isValid. вот мы говорили, что не стоит инкапсулировать просто так. нужно, чтобы констрейнты не нарушались. щас все геттеры и сеттеры абсолютно прозрачные. их могло бы и не быть и ничего бы не изменгилось
    ModelData &setTextureVertices(const QVector<QVector2D> &v);

    ModelData &setNormals(const QVector<QVector3D> &n);

    ModelData &setFaceVertexIndices(const QVector<int> &f);

    ModelData &setFaceTextureVertexIndices(const QVector<int> &f);

    ModelData &setFaceNormalIndices(const QVector<int> &f);

    ModelData &setPolygonStarts(const QVector<int> &p);

    bool isValid() const;

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

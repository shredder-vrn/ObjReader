#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>

struct Model {
    QVector<QVector3D> vertices;
    QVector<QVector2D> textureVertices;
    QVector<QVector3D> normals;

    QVector<int> faceVertexIndices;
    QVector<int> faceTextureVertexIndices;
    QVector<int> faceNormalIndices;

    QVector<int> polygonStarts;

    void clear();
    bool isValid() const;
};

#endif // MODEL_H

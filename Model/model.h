#ifndef MODELDATA_H
#define MODELDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMetaType>

//! REVIEW: namespace

struct Model
{
    QVector<QVector3D> vertices;
    QVector<QVector2D> textureVertices;
    QVector<QVector3D> normals;

    QVector<int> faceVertexIndices;
    QVector<int> faceTexCoordIndices;
    QVector<int> faceNormalIndices;

    QVector<int> polygonStarts;
    QVector<int> polygonLengths;
};

Q_DECLARE_METATYPE(Model)

#endif // MODELDATA_H

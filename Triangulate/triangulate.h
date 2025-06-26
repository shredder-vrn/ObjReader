#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <QVector3D>
#include <QVector>

void simpleTriangulateModel(
    const QVector<QVector3D> &vertices,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTextureVertexIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts);

void triangulateModel(
    const QVector<QVector3D>& vertices,
    QVector<int>& faceVertexIndices,
    QVector<int>& faceTextureVertexIndices,
    QVector<int>& faceNormalIndices,
    QVector<int>& polygonStarts);

#endif // TRIANGULATE_H

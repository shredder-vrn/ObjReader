#ifndef TRIANGULATE_H
#define TRIANGULATE_H


#include <QVector>
#include <QVector3D>

QVector<int> triangulatePolygon(
    const QVector<QVector3D>& allVertices,
    const QVector<int>& polygonVertexIndices);

#endif // TRIANGULATE_H

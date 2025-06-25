#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <QVector3D>
#include <QVector>

bool isInTriangle(const QVector3D& p, const QVector3D& a, const QVector3D& b, const QVector3D& c);
bool isConvex(const QVector<QVector3D>& allVertices, int prevIndex, int currIndex, int nextIndex);
bool isPolygonCCW(const QVector<QVector3D>& allVertices, const QVector<int>& polygonVertexIndices);
QVector<int> triangulatePolygon(const QVector<QVector3D>& allVertices, const QVector<int>& polygonVertexIndices);

#endif // TRIANGULATE_H

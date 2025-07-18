#ifndef VIEWER_TRIANGULATE_H
#define VIEWER_TRIANGULATE_H

#include <QVector3D>
#include <QVector>

namespace Viewer {

//! REIVIEW: а есть какой-то способ триангуляции без опоры на вершины? чисто по тоопологии?
void simpleTriangulateModel(
    const QVector<QVector3D> &vertices,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTextureVertexIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts);

void triangulateModel(
    const QVector<QVector3D> &vertices,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTextureVertexIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts);

}

#endif // VIEWER_TRIANGULATE_H

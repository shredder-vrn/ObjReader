#ifndef MODELDATA_H
#define MODELDATA_H

#include <QVector>
#include <QMetaType>

struct ModelV2
{
    QVector<float> vertexData;
    QVector<float> texCoordData;
    QVector<float> normalData;

    QVector<int> faceVertexIndices;
    QVector<int> faceTexCoordIndices;
    QVector<int> faceNormalIndices;

    QVector<int> polygonStarts;
    QVector<int> polygonLengths;
};

Q_DECLARE_METATYPE(ModelV2)

#endif // MODELDATA_H

#ifndef MESHDATA_H
#define MESHDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

struct Face {
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
};

struct Model {
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;
    QVector<Face> faces;
};

bool validateModel(const Model &model);

#endif // MESHDATA_H



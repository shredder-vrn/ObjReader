#ifndef MODEL_H
#define MODEL_H

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

#endif // MODEL_H

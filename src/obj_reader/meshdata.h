#ifndef MESHDATA_H
#define MESHDATA_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

struct Face
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;
};

struct Model
{
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;
    QVector<Face> faces;


//    QVector<QVector<int>> vertexIndicesPerFace;
//    QVector<QVector<int>> texCoordIndicesPerFace;
};

namespace MeshData{
    bool checkVertices(const Model &model);
    bool checkFaces(const Model &model);
    bool checkTexCords(const Model &model);
    bool checkNormals(const Model &model);
}

bool validateModel(const Model &model);

#endif // MESHDATA_H



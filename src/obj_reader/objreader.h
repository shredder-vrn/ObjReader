#ifndef OBJREADER_H
#define OBJREADER_H

#include "modeldata.h"


bool parseVertex(const QStringList &tokens, QVector<float> &vertexData, int &lineNum, const QString &line);
bool parseTexCoord(const QStringList &tokens, QVector<float> &texCoordData, int &lineNum, const QString &line);
bool parseNormal(const QStringList &tokens, QVector<float> &normalData, int &lineNum, const QString &line);
bool parseFace(
        const QStringList &tokens,
        QVector<int> &faceVertexIndices,
        QVector<int> &faceTexCoordIndices,
        QVector<int> &faceNormalIndices,
        QVector<int> &polygonStarts,
        QVector<int> &polygonLengths,
        int &lineNum,
        const QString &line);
bool checkVertices(const QVector<int> &vertexData);
bool checkTexCoords(
        QVector<float> &texCoordData,
        const QVector<int> &faceTexCoordIndices,
        const QVector<int> &polygonStarts,
        const QVector<int> &polygonLengths);
bool checkNormals(const ModelV2 &model);
bool checkFaces(const ModelV2 &model);
bool validateModel(const ModelV2 &model);
bool parseObj(const QString &filePath, ModelV2 &model);

#endif // OBJREADER_H

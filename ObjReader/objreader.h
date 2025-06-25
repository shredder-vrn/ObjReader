#ifndef OBJREADER_H
#define OBJREADER_H

#include "Model/model.h"

#include <QTextStream>

//! REVIEW nam

bool parseVertex(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector3D> &outVertices,
        int &outLineNum);
bool parseTexCoord(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector2D> &outTextureVertices,
        int &outLineNum);
bool parseNormal(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector3D> &outNormals,
        int &outLineNum);
bool parseFace(
        const QStringList &tokens,
        const QString &line,
        QVector<int> &faceVertexIndices,
        QVector<int> &faceTexCoordIndices,
        QVector<int> &faceNormalIndices,
        QVector<int> &polygonStarts,
        QVector<int> &polygonLengths,
        int &outLineNum);
bool checkVertices(
        const QVector<int> &vertices);
bool checkTexCoords(
        const QVector<int> &faceTexCoordIndices,
        const QVector<int> &polygonStarts,
        const QVector<int> &polygonLengths,
        QVector<QVector2D> &outTextureVertices);
bool checkNormals(
        const Model &model);
bool checkFaces(
        const Model &model);
bool validateModel(
        const Model &model);
bool parseTokens(
        QTextStream &in,
        Model &model);
bool parseObj(
        const QString &filePath,
        Model &model);


#endif // OBJREADER_H

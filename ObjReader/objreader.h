#ifndef OBJREADER_H
#define OBJREADER_H

#include "Model/model.h"
#include "Model/modeldata.h"

#include <QTextStream>

namespace Viewer {

bool parseVertex(const QStringList &tokens, QVector<QVector3D> &outVertices);

bool parseTexCoord(const QStringList &tokens, QVector<QVector2D> &outTextureVertices);

bool parseNormal(const QStringList &tokens, QVector<QVector3D> &outNormals);

bool validateModel(const Model &model);

bool parseTokens(QTextStream &in, Model &model);

bool parseObj(const QString &filePath, Model &model);

bool parseFace(
        const QString &line,
        QVector<int> &faceVertexIndices,
        QVector<int> &faceTextureVertexIndices,
        QVector<int> &faceNormalIndices,
        QVector<int> &polygonStarts);


bool parseTokens(QTextStream &in, ModelData &modelData);

bool parseObj(const QString &filePath, ModelData &modelData);

}

#endif // OBJREADER_H

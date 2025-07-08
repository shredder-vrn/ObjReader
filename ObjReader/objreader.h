#ifndef VIEWER_OBJREADER_H
#define VIEWER_OBJREADER_H

#include "Model/modeldata.h"

#include <QTextStream>

namespace Viewer {

bool parseVertex(const QStringList &tokens, QVector<QVector3D> &outVertices);

bool parseTexCoord(const QStringList &tokens, QVector<QVector2D> &outTextureVertices);

bool parseNormal(const QStringList &tokens, QVector<QVector3D> &outNormals);

//bool validateModel(const Model &model);

bool parseFace(
        const QString &line,
        QVector<int> &faceVertexIndices,
        QVector<int> &faceTextureVertexIndices,
        QVector<int> &faceNormalIndices,
        QVector<int> &polygonStarts);


bool parseTokens(QTextStream &in, ModelData &model, QString *errorLine = nullptr, int *errorLineNum = nullptr);

bool parseObj(const QString &filePath, ModelData &modelData);

}

#endif // VIEWER_OBJREADER_H

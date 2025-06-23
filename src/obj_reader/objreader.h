#ifndef OBJREADER_H
#define OBJREADER_H

#include "modeldata.h"


bool parseVertex(const QStringList &tokens, ModelV2 &model, int &lineNum, const QString &line);
bool parseTexCoord(const QStringList &tokens, ModelV2 &model, int &lineNum, const QString &line);
bool parseNormal(const QStringList &tokens, ModelV2 &model, int &lineNum, const QString &line);
bool parseFace(const QStringList &tokens, ModelV2 &model, int &lineNum, const QString &line);
bool checkVertices(const ModelV2 &model);
bool checkTexCoords(const ModelV2 &model);
bool checkNormals(const ModelV2 &model);
bool checkFaces(const ModelV2 &model);
bool validateModel(const ModelV2 &model);
bool parseObj(const QString &filePath, ModelV2 &model);

#endif // OBJREADER_H

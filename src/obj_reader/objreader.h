#ifndef OBJREADER_H
#define OBJREADER_H

#include "meshdata.h"

namespace ObjReader{
    bool parseVertexLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum);
    bool parseTexCoordLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum);
    bool parseNormalLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum);
    bool parseFaceLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum, QString &errorMessage);
}

bool parseObjvertices(const QString &filePath, Model &model);

#endif // OBJREADER_H

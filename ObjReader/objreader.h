#ifndef OBJREADER_H
#define OBJREADER_H

#include "Model/model.h"

bool parseObjvertices(
        const QString &filePath,
        QVector<QVector3D> &vertices,
        QVector<QVector2D> &texCoords,
        QVector<QVector3D> &normals,
        QVector<Face> &faces);


#endif // OBJREADER_H

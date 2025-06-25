#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QVector2D>

struct Vertex {
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};


#endif // VERTEX_H

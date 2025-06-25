#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions_4_1_Core>
#include <QVector>

#include "vertex.h"

class Mesh : protected QOpenGLFunctions_4_1_Core {
public:
    Mesh();
    ~Mesh();

    void init(const QVector<Vertex> &vertices, const QVector<unsigned int> &indices);
    void draw();

private:
    GLuint vao  = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLsizei indexCount = 0;
};

#endif // MESH_H

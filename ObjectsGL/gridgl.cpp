#include "gridgl.h"

namespace Viewer {

GridGL::GridGL(int size, float spacing) : m_size(size), m_spacing(spacing)
{
    initializeOpenGLFunctions();
    QVector<QVector3D> vertices;

    float halfSize = size * spacing / 2.0f;

    for (int i = 0; i <= size; ++i) {
        float x = -halfSize + i * spacing;
        vertices.append(QVector3D(x, 0, -halfSize));
        vertices.append(QVector3D(x, 0,  halfSize));

        float z = -halfSize + i * spacing;
        vertices.append(QVector3D(-halfSize, 0, z));
        vertices.append(QVector3D( halfSize, 0, z));
    }

    m_vertexCount = static_cast<GLuint>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GridGL::~GridGL()
{
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
}

GLuint GridGL::vao() const { return m_vao; }

GLuint GridGL::vertexCount() const { return m_vertexCount; }

GLuint GridGL::textureId() const { return 0; }

void GridGL::setVao(GLuint vao) { m_vao = vao; }

void GridGL::setVertexCount(GLuint count) { m_vertexCount = count; }

bool GridGL::useTexture() const { return false; }

bool GridGL::useNormals() const { return false; }

bool GridGL::isValid() const { return m_vao != 0; }

bool GridGL::hasNormals() const { return false; }

void GridGL::setHasTexture(bool) {}

void GridGL::setTextureId(GLuint) {}

void GridGL::setUseNormals(bool) {}

void GridGL::clear()
{
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }
    m_vertexCount = 0;
}

bool GridGL::operator!=(const ObjectGL &other) const
{
    const GridGL* otherGrid = dynamic_cast<const GridGL*>(&other);
    return !otherGrid || m_vao != otherGrid->m_vao || m_vertexCount != otherGrid->m_vertexCount;
}

}

#include "gridgl.h"

namespace Viewer {

GridGL::GridGL(float size)
{
    initializeOpenGLFunctions();

    std::vector<QVector3D> vertices = {
        { -size, 0, -size },
        {  size, 0, -size },
        {  size, 0,  size },
        { -size, 0, -size },
        {  size, 0,  size },
        { -size, 0,  size }
    };

    m_vertexCount = static_cast<GLuint>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(QVector3D)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);

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

#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <QOpenGLFunctions_3_3_Core>

struct Model {

    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_textureVertices;
    QVector<QVector3D> m_normals;

    QVector<int> m_faceVertexIndices;
    QVector<int> m_faceTextureVertexIndices;
    QVector<int> m_faceNormalIndices;
    QVector<int> m_polygonStarts;

    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    GLuint m_textureId = 0;

    bool m_hasTexture = false;
    bool m_useNormals = false;

    void clear();
    bool isValid() const;
    bool hasNormals() const { return !m_normals.isEmpty(); }
    bool operator!=(const Model& other) const;
};

#endif // MODEL_H

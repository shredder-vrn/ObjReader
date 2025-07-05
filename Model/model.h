#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector2D>
#include <QVector3D>

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>


class Model : protected QOpenGLFunctions
{
public:
    Model() = default;
    ~Model() = default;

    const QVector<QVector3D> &vertices() const;
    const QVector<QVector2D> &textureVertices() const;
    const QVector<QVector3D> &normals() const;

    const QVector<int> &faceVertexIndices() const;
    const QVector<int> &faceTextureVertexIndices() const;
    const QVector<int> &faceNormalIndices() const;
    const QVector<int> &polygonStarts() const;

    QVector<QVector3D> &setVertices();
    QVector<QVector2D> &setTextureVertices();
    QVector<QVector3D> &setNormals();

    QVector<int> &setFaceVertexIndices();
    QVector<int> &setFaceTextureVertexIndices();
    QVector<int> &setFaceNormalIndices();
    QVector<int> &setPolygonStarts();

    GLuint vao() const;
    GLuint vertexCount() const;
    GLuint textureId() const;

    void setVao(GLuint vao);
    void setVertexCount(GLuint count);

    bool hasTexture() const;
    bool useNormals() const;
    bool isValid() const;
    bool hasNormals() const;

    void setHasTexture(bool enabled);
    void setUseNormals(bool enabled);
    void setTextureId(GLuint id);

    void clear();

    bool operator!=(const Model& other) const;

private:
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
};

#endif // MODEL_H

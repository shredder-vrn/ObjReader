#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector3D>
#include <QVector2D>

#include <QOpenGLFunctions_3_3_Core>


struct Model {

    QVector<QVector3D> vertices;
    QVector<QVector2D> textureVertices;
    QVector<QVector3D> normals;

    QVector<int> faceVertexIndices;
    QVector<int> faceTextureVertexIndices;
    QVector<int> faceNormalIndices;

    QVector<int> polygonStarts;



    GLuint vao = 0;
    GLuint vertexCount = 0;

    GLuint textureId = 0;
    bool hasTexture = false;
    bool useNormal = false;

    void clear();
    bool isValid() const;
    bool operator!=(const Model& other) const;

};

#endif // MODEL_H

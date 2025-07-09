#ifndef VIEWER_MODELGL_H
#define VIEWER_MODELGL_H

#include <QVector>
#include <QDebug>
#include <QVector2D>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QQuaternion>

#include "Model/modeldata.h"
#include "ObjectsGL/objectsgl.h"

namespace Viewer {

class ModelGL : public ObjectGL
{
public:
    ModelGL();
    ~ModelGL();

    GLuint vao() const override;
    GLuint vertexCount() const override;
    GLuint textureId() const override;

    void setVao(GLuint vao) override;
    void setVertexCount(GLuint count) override;

    bool hasTexture() const override;
    bool isValid() const override;
    bool hasNormals() const override;


    void setHasTexture(bool enabled) override;
    void setTextureId(GLuint id) override;

    void clear() override;
    bool operator!=(const ObjectGL &other) const override;

    void setModelData(const ModelData *modelData);
    const ModelData *getModelData() const;

    void setUseNormals(bool enabled) override;
    bool useNormals() const override;

    void setWireframeMode(bool enabled);
    bool wireframeMode() const;

    QVector3D position() const;
    QQuaternion rotation() const;
    QVector3D scale() const;

    void setPosition(const QVector3D &pos);
    void setRotation(const QQuaternion &rot);
    void setScale(const QVector3D &scale);

private:
    ModelData m_modelData;

    GLuint m_vao = 0;
    GLuint m_vertexCount = 0;
    GLuint m_textureId = 0;

    bool m_hasTexture = false;
    bool m_useNormals = false;
    bool m_wireframeMode = false;

    QVector3D m_position = QVector3D(0, 0, 0);
    QQuaternion m_rotation = QQuaternion::fromAxisAndAngle(QVector3D(0, 1, 0), 0);
    QVector3D m_scale = QVector3D(1, 1, 1);

};
}

#endif // VIEWER_MODELGL_H

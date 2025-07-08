#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER

#include <QString>
#include <QMatrix4x4>

#include "Model/model.h"
#include "Model/modeldata.h"
#include "Model/modelgl.h"
namespace Viewer{
class ModelController {


public:
    ModelController() = default;
    ~ModelController() = default;

    QMatrix4x4 getModelMatrix() const;
    QVector<QMatrix4x4> const getModelMatrices(){return m_modelMatrices;}
    QString getErrorString() const { return errorString; }

    void translate(const QVector3D &translation);
    void rotate(float angle, const QVector3D &axis);
    void scale(const QVector3D &scalingFactors);
    void resetTransformations();
    void calculateNormals(ModelData &model);

    const ModelGL &getModelGL() const {return m_modelGL;};
    const QVector<ModelGL> &getModelGLs() const { return m_modelGLs; }

    bool loadModel(const QString &filePath, const int a);

private:
    QString errorString;
    QVector<QMatrix4x4> m_modelMatrices;
    QMatrix4x4 m_modelControllerModelMatrix;
    QVector<ModelGL> m_modelGLs;
    ModelGL m_modelGL;
    ModelData m_modelData;

};
}
#endif // MODEL_CONTROLLER

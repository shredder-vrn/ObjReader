#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER

#include <QString>
#include <QMatrix4x4>

#include "Model/model.h"
namespace Viewer{
class ModelController {

public:
    ModelController() = default;
    ~ModelController() = default;

    bool loadModel(const QString& filePath);

    const Model &getModel() const;
    QMatrix4x4 getModelMatrix() const;

    const QVector<Model> &getModels() const {return m_models;}
    QVector<QMatrix4x4> const getModelMatrices(){return m_modelMatrices;}

    void setTransform(int index, const QMatrix4x4 &transform);


    void translate(const QVector3D &translation);
    void rotate(float angle, const QVector3D &axis);
    void scale(const QVector3D &scalingFactors);
    void resetTransformations();

private:
    QVector<Model> m_models;
    QVector<QMatrix4x4> m_modelMatrices;


    Model modelControllerModel;
    QMatrix4x4 modelControllerModelMatrix;
};
}
#endif // MODEL_CONTROLLER

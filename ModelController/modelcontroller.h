#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER

#include <QString>
#include <QMatrix4x4>

#include "Model/model.h"

class ModelController {

public:
    ModelController() = default;
    ~ModelController() = default;

    bool loadModel(const QString& filePath);

    const Model &getModel() const;

    QMatrix4x4 getModelMatrix() const;

    void translate(const QVector3D &translation);
    void rotate(float angle, const QVector3D &axis);
    void scale(const QVector3D &scalingFactors);
    void resetTransformations();

private:
    Model modelControllerModel;
    QMatrix4x4 modelControllerModelMatrix;
};

#endif // MODEL_CONTROLLER

#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER

#include <QString>

#include "Model/model.h"

class ModelController {

public:
    ModelController() = default;
    ~ModelController() = default;

    bool loadModel(const QString& filePath);

    QMatrix4x4 getModelMatrix() const;

private:
    Model modelControllerModel;

    float modelControllerRotationX = 0.0f;
    float modelControllerRotationY = 0.0f;
    float modelControllerRotationZ = 0.0f;

};

#endif // MODEL_CONTROLLER

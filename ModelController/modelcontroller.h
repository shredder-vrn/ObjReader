#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER

#include <QString>

#include "Model/model.h"

class ModelController {
public:
    ModelController();
    ~ModelController() = default;

    bool loadModel(const QString& filePath);
    void clear();
    void rotate(float angleX, float angleY);

    QMatrix4x4 getModelMatrix() const;
    const Model &getModel() const;

private:
    Model model_;
    bool hasModel_ = false;
    float rotationX_ = 0.0f;
    float rotationY_ = 0.0f;
};

#endif // MODEL_CONTROLLER

#ifndef MODEL_CONTROLLER
#define MODEL_CONTROLLER


#include <memory>
#include <QString>
#include "Model/model.h"

class ModelController {
public:
    ModelController();
    ~ModelController();

    bool loadModel(const QString& filePath);
    void clear();
    void rotate(float angleX, float angleY);

    QMatrix4x4 getModelMatrix() const;
    const Model &getModel() const;

private:
    std::unique_ptr<Model> model;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
};

#endif // MODEL_CONTROLLER

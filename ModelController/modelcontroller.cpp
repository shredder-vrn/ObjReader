#include "modelcontroller.h"

#include <QMatrix4x4>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"

ModelController::ModelController() = default;

ModelController::~ModelController() = default;

bool ModelController::loadModel(const QString &filePath) {
    model.reset(new Model());
    if (!parseObj(filePath, *model)) {
        model.reset();
        return false;
    }
    simpleTriangulateModel(model->vertices,
                               model->faceVertexIndices,
                               model->faceTextureVertexIndices,
                               model->faceNormalIndices,
                               model->polygonStarts);
    return true;
}

void ModelController::clear() {
    model.reset();
    rotationX = 0.0f;
    rotationY = 0.0f;
}

void ModelController::rotate(float dx, float dy) {
    rotationX += dx;
    rotationY += dy;
}


QMatrix4x4 ModelController::getModelMatrix() const {
    QMatrix4x4 matrix;
    matrix.rotate(rotationX, 1.0f, 0.0f, 0.0f);
    matrix.rotate(rotationY, 0.0f, 1.0f, 0.0f);
    return matrix;
}


const Model &ModelController::getModel() const {
    static Model emptyModel;
    if (!model) return emptyModel;
    return *model;
}

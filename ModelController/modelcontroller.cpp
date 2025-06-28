#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QDebug>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"


ModelController :: ModelController(){
    qDebug() << "ModelController :: ModelController : запустили конструктор";

    qDebug() << "model.vertices" << model_.vertices;
    qDebug() << "hasModel_" << hasModel_;
    qDebug() << "rotationX_" << rotationX_;
    qDebug() << "rotationY_" << rotationY_;

    qDebug() << "ModelController :: ModelController : конструктор отработал, все преобразования обработаны.";
};


bool ModelController :: loadModel(const QString &filePath)
{
    qDebug() << "ModelController :: loadModel : запустили метод loadModel";

    Model newModel;

    if (!parseObj(filePath, newModel)) {
        return false;
    }

    triangulateModel(
                newModel.vertices,
                newModel.faceVertexIndices,
                newModel.faceTextureVertexIndices,
                newModel.faceNormalIndices,
                newModel.polygonStarts);

    model_ = newModel;
    hasModel_ = true;

    qDebug() << "ModelController :: loadModel : метод loadModel отработал";
    return true;
}

void ModelController :: clear()
{
    qDebug() << "ModelController :: clear : запустили метод clear";

    model_ = Model();
    hasModel_ = false;
    rotationX_ = 0.0f;
    rotationY_ = 0.0f;

    qDebug() << "ModelController :: clear : метод clear отработал";
}

void ModelController :: rotate(float dx, float dy)
{
    qDebug() << "ModelController :: rotate : запустили метод rotate";

    rotationX_ += dx;
    rotationY_ += dy;

    qDebug() << "ModelController :: rotate : метод rotate отработал";
}

QMatrix4x4 ModelController :: getModelMatrix() const
{
    qDebug() << "ModelController :: getModelMatrix : запустили метод getModelMatrix";

    QMatrix4x4 matrix;
    matrix.rotate(rotationX_, 1.0f, 0.0f, 0.0f);
    matrix.rotate(rotationY_, 0.0f, 1.0f, 0.0f);
    return matrix;


    qDebug() << "ModelController :: getModelMatrix : метод getModelMatrix отработал";
}

const Model &ModelController :: getModel() const
{
    qDebug() << "ModelController :: getModel : запустили метод getModel";

    if (!hasModel_) {
        static Model emptyModel;
        return emptyModel;
    }

    return model_;

    qDebug() << "ModelController :: getModel : запустили метод getModel";
}

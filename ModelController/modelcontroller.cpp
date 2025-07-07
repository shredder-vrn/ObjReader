#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QtMath>
#include <QDebug>
#include <QFile>

#include <cmath>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"

namespace Viewer{
bool ModelController::loadModel(const QString &filePath)
{
    Model newModel;

    QFile file(filePath);

    if (!file.exists()) {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    if (!parseObj(filePath, newModel)) {
        return false;
    }

    simpleTriangulateModel(
                newModel.vertices(),
                newModel.setFaceVertexIndices(),
                newModel.setFaceTextureVertexIndices(),
                newModel.setFaceNormalIndices(),
                newModel.setPolygonStarts());

    calculateNormals(newModel);

    m_modelControllerModel = newModel;
    m_models.append(newModel);
    m_modelMatrices.append(QMatrix4x4());

    return true;
}

const Model &ModelController::getModel() const
{
    return m_modelControllerModel;
}

QMatrix4x4 ModelController :: getModelMatrix() const
{
    return m_modelControllerModelMatrix;
}

void ModelController::translate(const QVector3D &translation)
{
    m_modelControllerModelMatrix.translate(translation);
}

void ModelController::rotate(float angle, const QVector3D &axis)
{
    m_modelControllerModelMatrix.rotate(angle, axis);
}

void ModelController::scale(const QVector3D &scalingFactors)
{
    m_modelControllerModelMatrix.scale(scalingFactors);
}

void ModelController::resetTransformations()
{
    m_modelControllerModelMatrix.setToIdentity();
}

void ModelController::calculateNormals(Model &model)
{
    if (model.vertices().isEmpty() || model.faceVertexIndices().size() % 3 != 0) {
        return;
    }

    model.setNormals().fill(QVector3D(0.0f, 0.0f, 0.0f), model.vertices().size());

    for (int i = 0; i < model.faceVertexIndices().size(); i += 3) {
        int idx0 = model.faceVertexIndices()[i];
        int idx1 = model.faceVertexIndices()[i + 1];
        int idx2 = model.faceVertexIndices()[i + 2];

        if (idx0 < 0 || idx1 < 0 || idx2 < 0 ||
            idx0 >= model.vertices().size() ||
            idx1 >= model.vertices().size() ||
            idx2 >= model.vertices().size()) {
            continue;
        }

        QVector3D v1 = model.vertices()[idx1] - model.vertices()[idx0];
        QVector3D v2 = model.vertices()[idx2] - model.vertices()[idx0];

        QVector3D normal = QVector3D::crossProduct(v2, v1).normalized();

        model.setNormals()[idx0] += normal;
        model.setNormals()[idx1] += normal;
        model.setNormals()[idx2] += normal;
    }

    for (auto& n : model.setNormals()) {
        n = n.normalized();
    }
}



const ModelData &ModelController::getModel(const int a) const
{

}

bool ModelController::loadModel(const QString &filePath, const int a)
{
    if (a == 0)
        qDebug() << "use new method loadModel from modelController";

    ModelData newData;

    QFile file(filePath);

    if (!file.exists())
        return false;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    if (!parseTokens(in, newData))
        return false;

//    simpleTriangulateModel(
//        newData.vertices(),
//        newData.setFaceVertexIndices(),
//        newData.setFaceTextureVertexIndices(),
//        newData.setFaceNormalIndices(),
//        newData.setPolygonStarts()
//    );

    calculateNormals(newData);

    m_modelGL.setModelData(&newData);

    m_modelData = newData;
    m_modelGLs.append(m_modelGL);
    m_modelMatrices.append(QMatrix4x4());

    return true;
}

void ModelController::calculateNormals(ModelData &model)
{

}
}


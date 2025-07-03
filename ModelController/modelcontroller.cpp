#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QtMath>
#include <QDebug>
#include <QFile>

#include <cmath>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"

namespace Viewer{
bool ModelController :: loadModel(const QString &filePath)
{
    qDebug() << "ModelController :: loadModel : запустили метод loadModel";

    Model newModel;

    QFile file(filePath);

    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filePath;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл для чтения:" << filePath;
        return false;
    }
    if (!parseObj(filePath, newModel)) {
        qCritical() << "Ошибка при разборе файла OBJ.";
        return false;
    }

    triangulateModel(
                newModel.vertices,
                newModel.faceVertexIndices,
                newModel.faceTextureVertexIndices,
                newModel.faceNormalIndices,
                newModel.polygonStarts);

    modelControllerModel = newModel;
    m_models.append(newModel);
    m_modelMatrices.append(QMatrix4x4());



    return true;
}

const Model &ModelController::getModel() const
{
    qDebug() << "ModelController :: getModel : запустили метод getModel";
    return modelControllerModel;
}

QMatrix4x4 ModelController :: getModelMatrix() const
{
    qDebug() << "ModelController :: getModelMatrix : запустили метод getModelMatrix";
    return modelControllerModelMatrix;
}

void ModelController::setTransform(int index, const QMatrix4x4 &transform)
{

}

void ModelController::translate(const QVector3D &translation)
{
    qDebug() << "ModelController :: translate : запустили метод translate";
    modelControllerModelMatrix.translate(translation);
}

void ModelController::rotate(float angle, const QVector3D &axis)
{
    qDebug() << "ModelController :: rotate : запустили метод rotate";
    modelControllerModelMatrix.rotate(angle, axis);
}

void ModelController::scale(const QVector3D &scalingFactors)
{
    qDebug() << "ModelController :: scale : запустили метод scale";
    modelControllerModelMatrix.scale(scalingFactors);
}

void ModelController::resetTransformations()
{
    modelControllerModelMatrix.setToIdentity();
}

}


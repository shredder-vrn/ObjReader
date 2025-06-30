#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QtMath>
#include <QDebug>
#include <QFile>

#include <cmath>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"


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

    return true;
}

QMatrix4x4 ModelController :: getModelMatrix() const
{
    qDebug() << "ModelController :: getModelMatrix : запустили метод getModelMatrix";

    QMatrix4x4 matrix;

    return matrix;
}


const Model &ModelController::getModel() const {
    qDebug() << "ModelController :: getModel : запустили метод getModel";

    return modelControllerModel;
}


#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QDebug>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"


bool ModelController :: loadModel(const QString &filePath)
{
    qDebug() << "ModelController :: loadModel : запустили метод loadModel";

    Model newModel;

    if (!parseObj(filePath, newModel))
        return false; 

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



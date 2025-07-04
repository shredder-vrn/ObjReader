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
    qDebug() << "ModelController :: loadModel : запустили метод loadModel";

    Model newModel;

    QFile file(filePath);

    if (!file.exists()) {
        qWarning() << "Файл не найден:" << filePath;
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для чтения:" << filePath;
        return false;
    }
    if (!parseObj(filePath, newModel)) {
        qWarning() << "Ошибка при разборе файла OBJ.";
        return false;
    }

    simpleTriangulateModel(
                newModel.m_vertices,
                newModel.m_faceVertexIndices,
                newModel.m_faceTextureVertexIndices,
                newModel.m_faceNormalIndices,
                newModel.m_polygonStarts);

    calculateNormals(newModel);

    m_modelControllerModel = newModel;
    m_models.append(newModel);
    m_modelMatrices.append(QMatrix4x4());

    return true;
}

const Model &ModelController::getModel() const
{
    qDebug() << "ModelController :: getModel : запустили метод getModel";
    return m_modelControllerModel;
}

QMatrix4x4 ModelController :: getModelMatrix() const
{
    qDebug() << "ModelController :: getModelMatrix : запустили метод getModelMatrix";
    return m_modelControllerModelMatrix;
}

void ModelController::setTransform(int index, const QMatrix4x4 &transform)
{

}

void ModelController::translate(const QVector3D &translation)
{
    qDebug() << "ModelController :: translate : запустили метод translate";
    m_modelControllerModelMatrix.translate(translation);
}

void ModelController::rotate(float angle, const QVector3D &axis)
{
    qDebug() << "ModelController :: rotate : запустили метод rotate";
    m_modelControllerModelMatrix.rotate(angle, axis);
}

void ModelController::scale(const QVector3D &scalingFactors)
{
    qDebug() << "ModelController :: scale : запустили метод scale";
    m_modelControllerModelMatrix.scale(scalingFactors);
}

void ModelController::resetTransformations()
{
    m_modelControllerModelMatrix.setToIdentity();
}

void ModelController::calculateNormals(Model &model)
    {
        if (model.m_vertices.isEmpty() || model.m_faceVertexIndices.size() % 3 != 0) {
            qDebug() << "[ERROR] Неверные данные модели для расчёта нормалей";
            return;
        }

        model.m_normals.fill(QVector3D(0.0f, 0.0f, 0.0f), model.m_vertices.size());

        for (int i = 0; i < model.m_faceVertexIndices.size(); i += 3) {
            int idx0 = model.m_faceVertexIndices[i];
            int idx1 = model.m_faceVertexIndices[i + 1];
            int idx2 = model.m_faceVertexIndices[i + 2];

            if (idx0 < 0 || idx1 < 0 || idx2 < 0 ||
                idx0 >= model.m_vertices.size() ||
                idx1 >= model.m_vertices.size() ||
                idx2 >= model.m_vertices.size()) {
                continue;
            }

            QVector3D v1 = model.m_vertices[idx1] - model.m_vertices[idx0];
            QVector3D v2 = model.m_vertices[idx2] - model.m_vertices[idx0];

            QVector3D normal = QVector3D::crossProduct(v2, v1).normalized();

            model.m_normals[idx0] += normal;
            model.m_normals[idx1] += normal;
            model.m_normals[idx2] += normal;
        }

        for (auto& n : model.m_normals) {
            n = n.normalized();
        }

        qDebug() << "[SUCCESS] Нормали рассчитаны";
    }

}


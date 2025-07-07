#include "modelcontroller.h"

#include <QMatrix4x4>
#include <QtMath>
#include <QDebug>
#include <QFile>

#include <cmath>

#include "ObjReader/objreader.h"
#include "Triangulate/triangulate.h"

namespace Viewer{

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

//*************************************
bool ModelController::loadModel(const QString &filePath, const int a)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    if (!parseTokens(in, m_modelData)) {
        return false;
    }

    if (m_modelData.vertices().isEmpty()) {
        return false;
    }

    QVector<int> faceVertexIndices = m_modelData.faceVertexIndices();
    QVector<int> faceTextureVertexIndices = m_modelData.faceTextureVertexIndices();
    QVector<int> faceNormalIndices = m_modelData.faceNormalIndices();
    QVector<int> polygonStarts = m_modelData.polygonStarts();

    simpleTriangulateModel(
        m_modelData.vertices(),
        faceVertexIndices,
        faceTextureVertexIndices,
        faceNormalIndices,
        polygonStarts
    );

    m_modelData
        .setFaceVertexIndices(faceVertexIndices)
        .setFaceTextureVertexIndices(faceTextureVertexIndices)
        .setFaceNormalIndices(faceNormalIndices)
        .setPolygonStarts(polygonStarts);

    calculateNormals(m_modelData);

    m_modelGL.setModelData(&m_modelData);
    m_modelGLs.append(m_modelGL);
    m_modelMatrices.append(QMatrix4x4());

    qDebug() << "[INFO] Загружено вершин:" << m_modelData.vertices().size();
    qDebug() << "[INFO] Загружено граней:" << m_modelData.faceVertexIndices().size() / 3;

    return true;
}

void ModelController::calculateNormals(ModelData &model)
{
    if (model.vertices().isEmpty() || model.faceVertexIndices().size() % 3 != 0) {
        return;
    }

    QVector<QVector3D> normals;
    normals.fill(QVector3D(0.0f, 0.0f, 0.0f), model.vertices().size());

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

        normals[idx0] += normal;
        normals[idx1] += normal;
        normals[idx2] += normal;
    }

    for (auto& n : normals) {
        n = n.normalized();
    }

    model.setNormals(normals);
}
//*************************************

}


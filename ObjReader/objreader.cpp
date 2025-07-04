#include "objreader.h"
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>
#include <QRegularExpression>
#include <QRegExp>
#include <QString>
#include "Model/model.h"

namespace Viewer {

bool parseVertex(
        const QStringList &tokens,
        QVector<QVector3D> &outVertices)
{
    if (tokens.size() < 4){
        return false;
    }
    if (tokens.size() > 4 && !tokens[4].startsWith('#')) {
        return false;
    }

    bool isXOk = false;
    bool isYOk = false;
    bool isZOk = false;
    float x = tokens[1].toFloat(&isXOk);
    float y = tokens[2].toFloat(&isYOk);
    float z = tokens[3].toFloat(&isZOk);

    if (!(isXOk && isYOk && isZOk)) {
        return false;
    }

    if (qIsNaN(x) || qIsNaN(y) || qIsNaN(z) || qIsInf(x) || qIsInf(y) || qIsInf(z)) {
        return false;
    }

    outVertices.append(QVector3D(x, y, z));
    return true;
}

bool parseTexCoord(
        const QStringList &tokens,
        QVector<QVector2D> &outTextureVertices)
{
    if (tokens.size() < 3) {
        return false;
    }

    if (tokens.size() > 3 && !tokens[3].startsWith('#')) {
        return false;
    }

    bool isUOk = false;
    bool isVOk = false;
    float u = tokens[1].toFloat(&isUOk);
    float v = tokens[2].toFloat(&isVOk);

    if (!isUOk || !isVOk){
        return false;
    }

    if (qIsNaN(u) || qIsNaN(v) ||qIsInf(u) || qIsInf(v)) {
        return false;
    }


    outTextureVertices.append(QVector2D(u, v));
    return true;
}

bool parseNormal(
        const QStringList &tokens,
        QVector<QVector3D> &outNormals)
{
    if (tokens.size() < 4){
        return false;
    }
    if (tokens.size() > 4 && !tokens[4].startsWith('#')) {
        return false;
    }

    bool isXOk = false;
    bool isYOk = false;
    bool isZOk = false;
    float x = tokens[1].toFloat(&isXOk);
    float y = tokens[2].toFloat(&isYOk);
    float z = tokens[3].toFloat(&isZOk);

    if (!(isXOk && isYOk && isZOk)){
        return false;
    }
    if (qIsNaN(x) || qIsNaN(y) || qIsNaN(z) ||qIsInf(x) || qIsInf(y) || qIsInf(z)) {
        return false;
    }

    outNormals.append(QVector3D(x, y, z));
    return true;
}

bool parseFace(
    const QString &line,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTexCoordIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts)
{
    QString cleanLine = line.section('#', 0, 0).trimmed();

    QRegularExpression re("\\s+");
    QStringList tokensCleaned = cleanLine.split(re, Qt::SkipEmptyParts); //QString::SkipEmptyParts

    if (tokensCleaned.isEmpty() || tokensCleaned[0] != "f") {
        return false;
    }

    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;

    int expectedFormat = -1;

    for (int i = 1; i < tokensCleaned.size(); ++i) {
        QStringList parts = tokensCleaned[i].split('/');

        if (parts.size() > 3) {
            return false;
        }

        int currentFormat = parts.size();

        if (expectedFormat == -1) {
            expectedFormat = currentFormat;
        }

        if (currentFormat != expectedFormat) {
            return false;
        }

        bool isOk = false;

        int vi = parts[0].trimmed().toInt(&isOk);
        if (!isOk || vi <= 0) {
            return false;
        }
        vertexIndices.append(vi - 1);

        int ti = -1;
        if (parts.size() >= 2 && !parts[1].trimmed().isEmpty()) {
            ti = parts[1].trimmed().toInt(&isOk);
            if (!isOk || ti <= 0) {
                ti = -1;
            }
        }

        std::vector<int> v = {1, 2, 3};
        texCoordIndices.append(ti == -1 ? -1 : ti - 1);

        int ni = -1;
        if (parts.size() >= 3 && !parts[2].trimmed().isEmpty()) {
            ni = parts[2].toInt(&isOk);
            if (!isOk || ni <= 0) {
                ni = -1;
            }
        }
        normalIndices.append(ni == -1 ? -1 : ni - 1);
    }

    if (vertexIndices.size() < 3) {
        return false;
    }

    int start = faceVertexIndices.size();
    polygonStarts.append(start);
    faceVertexIndices.append(vertexIndices);
    faceTexCoordIndices.append(texCoordIndices);
    faceNormalIndices.append(normalIndices);

    return true;
}


bool checkVertices(const QVector<QVector3D> &vertices)
{

    if (vertices.isEmpty()) {
        return false;
    }

    return true;
}

bool checkTexCoords(
        const QVector<QVector2D> &textureVertices,
        const QVector<int> &faceTexCoordIndices,
        const QVector<int> &polygonStarts)
{

    if (faceTexCoordIndices.isEmpty()) {
        return true;
    }

    int maxTexCoord = textureVertices.size();
    bool allValid = true;

    for (int i = 0; i < polygonStarts.size(); ++i) {
        int start = polygonStarts[i];
        int next = (i < polygonStarts.size() - 1) ? polygonStarts[i + 1] : faceTexCoordIndices.size();
        int count = next - start;

        if (start + count > faceTexCoordIndices.size()) {
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceTexCoordIndices[start + j];
            if (idx < 0 || idx >= maxTexCoord) {
                allValid = false;
            }
        }
    }
    return allValid;
}

bool checkNormals(
        const QVector<QVector3D> &normals,
        const QVector<int> &faceNormalIndices,
        const QVector<int> &polygonStarts)
{

    if (faceNormalIndices.isEmpty()) {
        return true;
    }

    int maxNormal = normals.size();
    bool allValid = true;

    for (int i = 0; i < polygonStarts.size(); ++i) {
        int start = polygonStarts[i];
        int next = (i < polygonStarts.size() - 1) ? polygonStarts[i + 1] : faceNormalIndices.size();
        int count = next - start;

        if (start + count > faceNormalIndices.size()) {
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceNormalIndices[start + j];
            if (idx < 0 || idx >= maxNormal) {
                allValid = false;
            }
        }
    }

    return allValid;
}

bool checkFaces(
        const QVector<QVector3D> &vertices,
        const QVector<int> &faceVertexIndices,
        const QVector<int> &polygonStarts)
{
    if (polygonStarts.isEmpty() || faceVertexIndices.isEmpty()) {
        return false;
    }

    int maxVertex = vertices.size();
    int totalIndices = 0;

    for (int i = 0; i < polygonStarts.size(); ++i) {
        int start = polygonStarts[i];
        int next = (i < polygonStarts.size() - 1) ? polygonStarts[i + 1] : faceVertexIndices.size();
        int count = next - start;

        if (count < 3) {
            return false;
        }

        if (start + count > faceVertexIndices.size()) {
            return false;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceVertexIndices[start + j];
            if (idx < 0 || idx >= maxVertex) {
                return false;
            }
        }

        totalIndices += count;
    }

    if (totalIndices != faceVertexIndices.size()) {
        return false;
    }

    return true;
}

bool validateModel(const Model &model)
{
    bool verticesOk = checkVertices(model.m_vertices);
    bool texCoordsOk = checkTexCoords(model.m_textureVertices, model.m_faceTextureVertexIndices, model.m_polygonStarts);
    bool normalsOk = checkNormals(model.m_normals, model.m_faceNormalIndices, model.m_polygonStarts);
    bool facesOk = checkFaces(model.m_vertices, model.m_faceVertexIndices, model.m_polygonStarts);
    return verticesOk && texCoordsOk && normalsOk && facesOk;
}

bool parseTokens(QTextStream &in, Model &model)
{
    qDebug() << "parseTokens :: parseTokens : запустили метод parseTokens";
    int lineNum = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        ++lineNum;

        if (line.isEmpty()) {
            continue;
        }
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) {
            continue;
        }
        const QString type = tokens[0];

        if (type == "#")
            continue;
        if (type == "v" && !parseVertex(tokens, model.m_vertices))
            return false;
        if (type == "vt" && !parseTexCoord(tokens, model.m_textureVertices))
            return false;
        if (type == "vn" && !parseNormal(tokens, model.m_normals))
            return false;
        if (type == "f" && !parseFace(line, model.m_faceVertexIndices, model.m_faceTextureVertexIndices, model.m_faceNormalIndices, model.m_polygonStarts))
            return false;
    }

    qDebug() << "parseTokens :: parseTokens : метод parseTokens отработал";

    return true;
}

bool parseObj(const QString &filePath, Model &model)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    return parseTokens(in, model);
}

}

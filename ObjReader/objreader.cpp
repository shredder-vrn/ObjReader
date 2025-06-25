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
    QVector<int> &polygonStarts,
    QVector<int> &polygonLengths)
{
    QString cleanLine = line.section('#', 0, 0);

    QRegularExpression re("\\s+");
    QStringList tokensCleaned = cleanLine.split(re, QString::SkipEmptyParts);

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

        if (currentFormat != expectedFormat) {
            return false;
        }

        expectedFormat = currentFormat;

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
        texCoordIndices.append(ti - 1);

        int ni = -1;
        if (parts.size() >= 3 && !parts[2].trimmed().isEmpty()) {
            ni = parts[2].toInt(&isOk);
            if (!isOk || ni <= 0) {
                ni = -1;
            }
        }
        normalIndices.append(ni - 1);
    }

    if (vertexIndices.size() < 3) {
        return false;
    }

    int start = faceVertexIndices.size();
    polygonStarts.append(start);
    polygonLengths.append(vertexIndices.size());

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
        const QVector<QVector2D> &texCoordData,
        const QVector<int> &faceTexCoordIndices,
        const QVector<int> &polygonStarts,
        const QVector<int> &polygonLengths)
{

    if (faceTexCoordIndices.isEmpty()) {
        return true;
    }

    int maxTexCoord = texCoordData.size();
    bool allValid = true;

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];

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
        const QVector<QVector3D> &normalData,
        const QVector<int> &faceNormalIndices,
        const QVector<int> &polygonStarts,
        const QVector<int> &polygonLengths)
{

    if (faceNormalIndices.isEmpty()) {
        return true;
    }

    int maxNormal = normalData.size();
    bool allValid = true;

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];

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
        const QVector<QVector3D> &vertexData,
        const QVector<int> &faceVertexIndices,
        const QVector<int> &polygonStarts,
        const QVector<int> &polygonLengths)
{

    if (polygonStarts.isEmpty() || polygonLengths.isEmpty()) {
        return false;
    }

    if (polygonStarts.size() != polygonLengths.size()) {
        return false;
    }

    int totalIndices = 0;
    int maxVertex = vertexData.size();

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];


        if (count < 3) {
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
    bool verticesOk = checkVertices(model.vertices);
    bool texCoordsOk = checkTexCoords(model.textureVertices, model.faceTexCoordIndices, model.polygonStarts, model.polygonLengths);
    bool normalsOk = checkNormals(model.normals, model.faceNormalIndices, model.polygonStarts, model.polygonLengths);
    bool facesOk = checkFaces(model.vertices, model.faceVertexIndices, model.polygonStarts, model.polygonLengths);
    bool result = verticesOk && texCoordsOk && normalsOk && facesOk;
    return result;
}

bool parseTokens(QTextStream &in, Model &model)
{
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
        if (type == "v" && !parseVertex(tokens, line, model.vertices, lineNum))
            return false;
        if (type == "vt" && !parseTexCoord(tokens, line, model.textureVertices, lineNum))
            return false;
        if (type == "vn" && !parseNormal(tokens, line, model.normals, lineNum))
            return false;
        if (type == "f" && !parseFace(line, model.faceVertexIndices, model.faceTexCoordIndices, model.faceNormalIndices, model.polygonStarts, model.polygonLengths, lineNum))
            return false;
    }
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

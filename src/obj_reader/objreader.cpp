#include "objreader.h"
#include "modeldata.h"

#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

enum class ParseError
{
    None,
    FileOpenFailed,
    InvalidVertexLine,
    VertexNotEnoughCoords,
    VertexTooManyCoords,
    VertexInvalidNumber,
    TexCoordNotEnoughCoords,
    TexCoordInvalidNumber,
    NormalNotEnoughCoords,
    NormalTooManyCoords,
    NormalInvalidNumber,
    FaceInvalidIndexFormat,
    FaceMissingVertexIndex,
    FaceVertexIndexInvalid,
    FaceTexCoordIndexInvalid,
    FaceNormalIndexInvalid,
    FaceLessThanThreeVertices,
    FaceIndexOutOfBounds,
    EmptyData,
    CorruptedDataStructure
};

QString errorMessage (ParseError code)
{
    switch (code) {
        case ParseError::None: return "No error.";
        case ParseError::FileOpenFailed: return "Failed to open file.";
        case ParseError::InvalidVertexLine: return "Invalid vertex line.";
        case ParseError::VertexNotEnoughCoords: return "Vertex line has not enough coordinates.";
        case ParseError::VertexTooManyCoords: return "Vertex line has too many coordinates.";
        case ParseError::VertexInvalidNumber: return "Vertex coordinate is not a number.";
        case ParseError::TexCoordNotEnoughCoords: return "Texture coordinate line has not enough values.";
        case ParseError::TexCoordInvalidNumber: return "Texture coordinate value is not a number.";
        case ParseError::NormalNotEnoughCoords: return "Normal line has not enough coordinates.";
        case ParseError::NormalTooManyCoords: return "Normal line has too many coordinates.";
        case ParseError::NormalInvalidNumber: return "Normal coordinate is not a number.";
        case ParseError::FaceInvalidIndexFormat: return "Face index format is invalid.";
        case ParseError::FaceMissingVertexIndex: return "Face missing vertex index.";
        case ParseError::FaceVertexIndexInvalid: return "Face vertex index is invalid.";
        case ParseError::FaceTexCoordIndexInvalid: return "Face texture coordinate index is invalid.";
        case ParseError::FaceNormalIndexInvalid: return "Face normal index is invalid.";
        case ParseError::FaceLessThanThreeVertices: return "Face has less than three vertices.";
        case ParseError::FaceIndexOutOfBounds: return "Face index out of bounds.";
        case ParseError::EmptyData: return "Data is empty.";
        case ParseError::CorruptedDataStructure: return "Data structure is corrupted.";
        default: return "Unknown error.";
    }
}

void logError (
        ParseError error,
        int lineNum = -1,
        const QString &line = " ")
{
    QString msg = errorMessage(error);
    if (lineNum != -1)
        msg += QString(" Line %1").arg(lineNum);
    if (!line.isEmpty())
        msg += QString(": '%1'").arg(line);
    qWarning() << msg;
}

bool parseVertex(
        const QStringList &tokens,
        QVector<float> &vertexData,
        int &lineNum,
        const QString &line)
{
    if (tokens.size() < 4){
        logError(ParseError::VertexNotEnoughCoords,lineNum, line);
        return false;
    }

    if (tokens.size() > 4){
        logError(ParseError::VertexTooManyCoords,lineNum, line);
        return false;
    }

    bool okX = false;
    bool okY = false;
    bool okZ = false;

    float x = tokens[1].toFloat(&okX);
    float y = tokens[2].toFloat(&okY);
    float z = tokens[3].toFloat(&okZ);

    if (!okX || !okY || !okZ){
        logError(ParseError::VertexInvalidNumber, lineNum, line);
        return false;
    }

    vertexData.append(x);
    vertexData.append(y);
    vertexData.append(z);
    return true;
}

bool parseTexCoord(
        const QStringList &tokens,
        QVector<float> &texCoordData,
        int &lineNum,
        const QString &line)
{

    if (tokens.size() < 3) {
        logError(ParseError::TexCoordNotEnoughCoords, lineNum, line);
        return false;
    }

    bool okU = false;
    bool okV = false;

    float u = tokens[1].toFloat(&okU);
    float v = tokens[2].toFloat(&okV);

    if (!okU || !okV){
        logError(ParseError::TexCoordInvalidNumber, lineNum, line);
        return false;
    }

    texCoordData.append(u);
    texCoordData.append(v);
    return true;
}

bool parseNormal(
        const QStringList &tokens,
        QVector<float> &normalData,
        int &lineNum,
        const QString &line)
{

    if (tokens.size() < 4){
        logError(ParseError::NormalNotEnoughCoords, lineNum, line);
        return false;
    }

    if (tokens.size() > 4){
        logError(ParseError::NormalTooManyCoords, lineNum, line);
        return false;
    }

    bool okX = false;
    bool okY = false;
    bool okZ = false;

    float x = tokens[1].toFloat(&okX);
    float y = tokens[2].toFloat(&okY);
    float z = tokens[3].toFloat(&okZ);

    if (!okX || !okY || !okZ){
        logError(ParseError::NormalInvalidNumber, lineNum, line);
        return false;
    }

    normalData.append(x);
    normalData.append(y);
    normalData.append(z);
    return true;
}

bool parseFace(
        const QStringList &tokens,
        QVector<int> &faceVertexIndices,
        QVector<int> &faceTexCoordIndices,
        QVector<int> &faceNormalIndices,
        QVector<int> &polygonStarts,
        QVector<int> &polygonLengths,
        int &lineNum,
        const QString &line)
{
    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;

    for (int i = 1; i < tokens.size(); ++i){
        QStringList parts = tokens[i].split('/');
        if (parts.isEmpty()) {
            logError(ParseError::FaceMissingVertexIndex, lineNum, line);
            return false;
        }

        bool ok = false;
        int vi = parts[0].toInt(&ok);
        if (!ok || vi <= 0) {
            logError(ParseError::FaceVertexIndexInvalid, lineNum, line);
            return false;
        }
        vertexIndices.append(vi - 1);

        int ti = -1;
        if (parts.size() >= 2 && !parts[1].isEmpty()) {
            ti = parts[1].toInt(&ok);
            if (!ok || ti <= 0) {
                logError(ParseError::FaceTexCoordIndexInvalid, lineNum, line);
                ti = -1;
            }
        }
        texCoordIndices.append(ti - 1);

        int ni = -1;
        if (parts.size() >= 3 && !parts[2].isEmpty()) {
            ni = parts[2].toInt(&ok);
            if (!ok || ni <= 0) {
                logError(ParseError::FaceNormalIndexInvalid, lineNum, line);
                ni = -1;
            }
        }
        normalIndices.append(ni - 1);
    }

    if (vertexIndices.size() < 3) {
        logError(ParseError::FaceLessThanThreeVertices, lineNum, line);
        return false;
    }

    int start = faceVertexIndices.size();
    faceVertexIndices.append(vertexIndices);
    faceTexCoordIndices.append(texCoordIndices);
    faceNormalIndices.append(normalIndices);
    polygonStarts.append(start);
    polygonLengths.append(vertexIndices.size());

    return true;
}


bool checkVertices(const ModelV2 &model)
{
    if (model.vertexData.isEmpty()) {
        logError(ParseError::EmptyData);
        return false;
    }
    if (model.vertexData.size() % 3 != 0) {
        logError(ParseError::CorruptedDataStructure);
        return false;
    }
    return true;
}

bool checkTexCoords(const ModelV2 &model)
{
    if (model.texCoordData.isEmpty()) {
        if (!model.faceTexCoordIndices.isEmpty()) {
            logError(ParseError::CorruptedDataStructure);
            return false;
        }
        return true;
    }

    if (model.texCoordData.size() % 2 != 0) {
        logError(ParseError::CorruptedDataStructure);
        return false;
    }

    if (model.faceTexCoordIndices.isEmpty()) {
        return true;
    }

    int maxTexCoord = model.texCoordData.size() / 2;
    bool allValid = true;

    for (int i = 0; i < model.polygonLengths.size(); ++i) {

        int start = model.polygonStarts[i];
        int count = model.polygonLengths[i];

        if (start + count > model.faceTexCoordIndices.size()) {
            logError(ParseError::FaceIndexOutOfBounds);
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = model.faceTexCoordIndices[start + j];
            if (idx < 0 || idx >= maxTexCoord) {
                logError(ParseError::FaceIndexOutOfBounds);
                allValid = false;
            }
        }
    }
    return allValid;
}

bool checkNormals(const ModelV2 &model)
{
    if (model.normalData.isEmpty()) {
        if (!model.faceNormalIndices.isEmpty()) {
            logError(ParseError::CorruptedDataStructure);
            return false;
        }
        return true;
    }

    if (model.normalData.size() % 3 != 0) {
        logError(ParseError::CorruptedDataStructure);
        return false;
    }

    if (!model.faceNormalIndices.isEmpty()) {
        return true;
    }

    int maxNormal = model.normalData.size() / 3;
    bool allValid = true;


    for (int i = 0; i < model.polygonLengths.size(); ++i) {

        int start = model.polygonStarts[i];
        int count = model.polygonLengths[i];

        if (start + count > model.faceNormalIndices.size()) {
            logError(ParseError::FaceIndexOutOfBounds);
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = model.faceNormalIndices[start + j];
            if (idx < 0 || idx >= maxNormal) {
                logError(ParseError::FaceIndexOutOfBounds);
                allValid = false;
            }
        }
    }
    return allValid;
}

bool checkFaces(const ModelV2 &model)
{
    if (model.polygonStarts.isEmpty() || model.polygonLengths.isEmpty()) {
        logError(ParseError::EmptyData);
        return false;
    }
    if (model.polygonStarts.size() != model.polygonLengths.size()) {
        logError(ParseError::CorruptedDataStructure);
        return false;
    }

    int totalIndices = 0;
    int maxVertex = model.vertexData.size() / 3;
    for (int i = 0; i < model.polygonLengths.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = model.polygonLengths[i];
        if (count < 3) {
            logError(ParseError::FaceLessThanThreeVertices);
            return false;
        }
        for (int j = 0; j < count; ++j) {
            int idx = model.faceVertexIndices[start + j];
            if (idx < 0 || idx >= maxVertex) {
                logError(ParseError::FaceIndexOutOfBounds);
                return false;
            }
        }
        totalIndices += count;
    }

    if (totalIndices != model.faceVertexIndices.size()) {
        logError(ParseError::CorruptedDataStructure);
        return false;
    }

    return true;
}

bool validateModel(const ModelV2 &model)
{
    return checkVertices(model) &&
           checkTexCoords(model) &&
           checkNormals(model) &&
           checkFaces(model);
}

bool parseObj(
        const QString &filePath,
        ModelV2 &model)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError(ParseError::FileOpenFailed);
        return false;
    }

    QTextStream in(&file);
    int lineNum = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        ++lineNum;
        if (line.isEmpty()) continue;

        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);
        if (tokens.isEmpty()) continue;

        QString type = tokens[0];

        /*
        // --- DEBUG: до обработки строки ---
        qDebug() << "\n[DEBUG] Строка" << lineNum << "(" << type << ")";
        qDebug() << "  Исходная строка:" << line;
        qDebug() << "  vertexData:" << model.vertexData;
        qDebug() << "  texCoordData:" << model.texCoordData;
        qDebug() << "  normalData:" << model.normalData;
        qDebug() << "  faceVertexIndices:" << model.faceVertexIndices;
        qDebug() << "  faceTexCoordIndices:" << model.faceTexCoordIndices;
        qDebug() << "  faceNormalIndices:" << model.faceNormalIndices;
        qDebug() << "  polygonStarts:" << model.polygonStarts;
        qDebug() << "  polygonLengths:" << model.polygonLengths;
        */
        if (type == "#") continue;
        else if (type == "v" && !parseVertex(tokens, model.vertexData, lineNum, line))
            return false;
        else if (type == "vt" && !parseTexCoord(tokens, model.texCoordData, lineNum, line))
            return false;
        else if (type == "vn" && !parseNormal(tokens, model.normalData, lineNum, line))
            return false;
        else if (type == "f" && !parseFace(
                     tokens,
                     model.faceVertexIndices,
                     model.faceTexCoordIndices,
                     model.faceNormalIndices,
                     model.polygonStarts,
                     model.polygonLengths,
                     lineNum,
                     line))
            return false;

        qDebug() << "\n[DEBUG] После обработки строки" << lineNum << ":" << line;
        qDebug() << "  vertexData.size()" << model.vertexData.size();
        qDebug() << "  texCoordData.size()" << model.texCoordData.size();
        qDebug() << "  normalData.size()" << model.normalData.size();
        qDebug() << "  faceVertexIndices.size()" << model.faceVertexIndices.size();
        qDebug() << "  faceTexCoordIndices.size()" << model.faceTexCoordIndices.size();
        qDebug() << "  faceNormalIndices.size()" << model.faceNormalIndices.size();
        qDebug() << "  polygonStarts.size()" << model.polygonStarts.size();
        qDebug() << "  polygonLengths.size()" << model.polygonLengths.size();

        qDebug() << "--------------------------------------------------";

    }

    return validateModel(model);
}

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
    FaceContainsComment,
    FaceInvalidCommand,
    FaceInvalidIndexFormat,
    FaceInvalidVertexFormat,
    FaceMixedVertexFormats,
    FaceMissingVertexIndex,
    FaceVertexIndexInvalid,
    FaceTexCoordIndexInvalid,
    FaceNormalIndexInvalid,
    FaceLessThanThreeVertices,
    FaceIndexOutOfBounds,
    EmptyData,
    CorruptedDataStructure
};

QString errorMessageFromCode(const ParseError code)
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
        case ParseError::FaceContainsComment: return "Face contains comment is not a valid.";
        case ParseError::FaceInvalidCommand: return "Face command is not a valid.";
        case ParseError::FaceInvalidIndexFormat: return "Face index format is invalid.";
        case ParseError::FaceInvalidVertexFormat: return "Face index format is invalid.";
        case ParseError::FaceMixedVertexFormats: return "Face index format is invalid.";
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

void buildLogErrorAndPrint (
        const ParseError error,
        const int lineNum = -1,
        const QString &line = " ")
{
    QString msg = errorMessageFromCode(error);
    if (lineNum != -1)
        msg += QString(" Line %1").arg(lineNum);
    if (!line.isEmpty())
        msg += QString(": '%1'").arg(line);
    qWarning() << msg;
}

bool parseVertex(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector3D> &outVertices,
        int &outLineNum)
{
    if (tokens.size() < 4){
        buildLogErrorAndPrint(ParseError::VertexNotEnoughCoords, outLineNum, line);
        return false;
    }
    if (tokens.size() > 4 && !tokens[4].startsWith('#')) {
        buildLogErrorAndPrint(ParseError::VertexTooManyCoords, outLineNum, line);
        return false;
    }

    bool isXOk = false;
    bool isYOk = false;
    bool isZOk = false;
    float x = tokens[1].toFloat(&isXOk);
    float y = tokens[2].toFloat(&isYOk);
    float z = tokens[3].toFloat(&isZOk);

    if (!(isXOk && isYOk && isZOk)) {
        buildLogErrorAndPrint(ParseError::VertexInvalidNumber, outLineNum, line);
        return false;
    }

    if (qIsNaN(x) || qIsNaN(y) || qIsNaN(z) ||qIsInf(x) || qIsInf(y) || qIsInf(z)) {
        buildLogErrorAndPrint(ParseError::VertexInvalidNumber, outLineNum, line);
        return false;
    }

    outVertices.append(QVector3D(x, y, z));
    return true;
}

bool parseTexCoord(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector2D> &outTextureVertices,
        int &outLineNum)
{
    if (tokens.size() < 3) {
        buildLogErrorAndPrint(ParseError::TexCoordNotEnoughCoords, outLineNum, line);
        return false;
    }

    if (tokens.size() > 3 && !tokens[3].startsWith('#')) {
        buildLogErrorAndPrint(ParseError::VertexTooManyCoords, outLineNum, line);
        return false;
    }

    bool isUOk = false;
    bool isVOk = false;
    float u = tokens[1].toFloat(&isUOk);
    float v = tokens[2].toFloat(&isVOk);



    if (!isUOk || !isVOk){
        buildLogErrorAndPrint(ParseError::TexCoordInvalidNumber, outLineNum, line);
        return false;
    }

    if (qIsNaN(u) || qIsNaN(v) ||qIsInf(u) || qIsInf(v)) {
        buildLogErrorAndPrint(ParseError::VertexInvalidNumber, outLineNum, line);
        return false;
    }


    outTextureVertices.append(QVector2D(u, v));
    return true;
}

bool parseNormal(
        const QStringList &tokens,
        const QString &line,
        QVector<QVector3D> &outNormals,
        int &outLineNum)
{
    if (tokens.size() < 4){
        buildLogErrorAndPrint(ParseError::NormalNotEnoughCoords, outLineNum, line);
        return false;
    }
    if (tokens.size() > 4 && !tokens[4].startsWith('#')) {
        buildLogErrorAndPrint(ParseError::VertexTooManyCoords, outLineNum, line);
        return false;
    }

    bool isXOk = false;
    bool isYOk = false;
    bool isZOk = false;
    float x = tokens[1].toFloat(&isXOk);
    float y = tokens[2].toFloat(&isYOk);
    float z = tokens[3].toFloat(&isZOk);

    if (!(isXOk && isYOk && isZOk)){
        buildLogErrorAndPrint(ParseError::NormalInvalidNumber, outLineNum, line);
        return false;
    }
    if (qIsNaN(x) || qIsNaN(y) || qIsNaN(z) ||qIsInf(x) || qIsInf(y) || qIsInf(z)) {
        buildLogErrorAndPrint(ParseError::VertexInvalidNumber, outLineNum, line);
        return false;
    }

    outNormals.append(QVector3D(x, y, z));
    return true;
}



#include <QDebug>
#include <QRegularExpression>

bool parseFace(
    const QStringList &tokens,
    const QString &line,
    QVector<int> &faceVertexIndices,
    QVector<int> &faceTexCoordIndices,
    QVector<int> &faceNormalIndices,
    QVector<int> &polygonStarts,
    QVector<int> &polygonLengths,
    int &outLineNum)
{
    qDebug() << "=== Начало парсинга грани ===";

    // --- Шаг 1: Удаление комментариев ---
    QString cleanLine = line.section('#', 0, 0);
    qDebug() << "[DEBUG] После удаления комментария:" << cleanLine;

    // --- Шаг 2: Разбиение на токены ---
    QRegularExpression re("\\s+");
    QStringList tokensCleaned = cleanLine.split(re, QString::SkipEmptyParts);
    qDebug() << "[DEBUG] Токены после split:" << tokensCleaned;

    if (tokensCleaned.isEmpty() || tokensCleaned[0] != "f") {
        buildLogErrorAndPrint(ParseError::FaceInvalidCommand, outLineNum, line);
        return false;
    }

    QVector<int> vertexIndices;
    QVector<int> texCoordIndices;
    QVector<int> normalIndices;

    int expectedFormat = -1; // Проверка единообразия формата

    // --- Цикл по вершинам ---
    for (int i = 1; i < tokensCleaned.size(); ++i) {
        qDebug() << "[DEBUG] Парсим вершину #" << i << ":" << tokensCleaned[i];

        QStringList parts = tokensCleaned[i].split('/');
        qDebug() << "        Разделено на части:" << parts;


        // --- Проверяем, что вершина имеет максимум 3 компонента ---
        if (parts.size() > 3) {
            qDebug() << "[ERROR] Слишком много частей в вершине:" << parts;
            buildLogErrorAndPrint(ParseError::FaceInvalidVertexFormat, outLineNum, line);
            return false;
        }


        // --- Проверяем единообразие форматов ---
        int currentFormat = parts.size();
        if (expectedFormat == -1) {
            expectedFormat = currentFormat;
            qDebug() << "[DEBUG] Установлен ожидаемый формат:" << expectedFormat;
        } else if (currentFormat != expectedFormat) {
            qDebug() << "[ERROR] Неоднородные форматы вершин.";
            buildLogErrorAndPrint(ParseError::FaceMixedVertexFormats, outLineNum, line);
            return false;
        }

        bool isOk = false;

        // --- Парсим вершину ---
        int vi = parts[0].toInt(&isOk);
        if (!isOk || vi <= 0) {
            qDebug() << "[ERROR] Вершина невалидна:" << parts[0];
            buildLogErrorAndPrint(ParseError::FaceVertexIndexInvalid, outLineNum, line);
            return false;
        }
        vertexIndices.append(vi - 1); // OpenGL-style indexing
        qDebug() << "        vertexIndices =" << vertexIndices;

        // --- Парсим текстурную координату ---
        int ti = -1;
        if (parts.size() >= 2) {
            if (!parts[1].isEmpty()) {
                ti = parts[1].toInt(&isOk);
                if (!isOk || ti <= 0) {
                    qDebug() << "[ERROR] Texture coord невалиден:" << parts[1];
                    buildLogErrorAndPrint(ParseError::FaceTexCoordIndexInvalid, outLineNum, line);
                    ti = -1;
                }
            }
        }
        texCoordIndices.append(ti - 1);
        qDebug() << "        texCoordIndices =" << texCoordIndices;

        // --- Парсим нормаль ---
        int ni = -1;
        if (parts.size() >= 3) {
            if (!parts[2].isEmpty()) {
                ni = parts[2].toInt(&isOk);
                if (!isOk || ni <= 0) {
                    qDebug() << "[ERROR] Нормаль невалидна:" << parts[2];
                    buildLogErrorAndPrint(ParseError::FaceNormalIndexInvalid, outLineNum, line);
                    ni = -1;
                }
            }
        }
        normalIndices.append(ni - 1);
        qDebug() << "        normalIndices =" << normalIndices;
    }

    // --- Проверка количества вершин ---
    if (vertexIndices.size() < 3) {
        qDebug() << "[ERROR] Меньше трёх вершин";
        buildLogErrorAndPrint(ParseError::FaceLessThanThreeVertices, outLineNum, line);
        return false;
    }

    // --- Сохранение результата ---
    int start = faceVertexIndices.size();
    faceVertexIndices.append(vertexIndices);
    faceTexCoordIndices.append(texCoordIndices);
    faceNormalIndices.append(normalIndices);
    polygonStarts.append(start);
    polygonLengths.append(vertexIndices.size());

    qDebug() << "[INFO] Грань успешно добавлена:";
    qDebug() << "        faceVertexIndices:" << faceVertexIndices;
    qDebug() << "        faceTexCoordIndices:" << faceTexCoordIndices;
    qDebug() << "        faceNormalIndices:" << faceNormalIndices;
    qDebug() << "        polygonStarts:" << polygonStarts;
    qDebug() << "        polygonLengths:" << polygonLengths;

    return true;
}


bool checkVertices(const QVector<QVector3D> &vertices)
{
    if (vertices.isEmpty()) {
        buildLogErrorAndPrint(ParseError::EmptyData);
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
    Q_UNUSED(texCoordData); // больше не нужна для проверок длины (%2)

    if (faceTexCoordIndices.isEmpty()) {
        return true;
    }

    int maxTexCoord = texCoordData.size();
    bool allValid = true;

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];

        if (start + count > faceTexCoordIndices.size()) {
            buildLogErrorAndPrint(ParseError::FaceIndexOutOfBounds);
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceTexCoordIndices[start + j];
            if (idx < 0 || idx >= maxTexCoord) {
                buildLogErrorAndPrint(ParseError::FaceIndexOutOfBounds);
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
    Q_UNUSED(normalData); // аналогично

    if (faceNormalIndices.isEmpty()) {
        return true;
    }

    int maxNormal = normalData.size();
    bool allValid = true;

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];

        if (start + count > faceNormalIndices.size()) {
            buildLogErrorAndPrint(ParseError::FaceIndexOutOfBounds);
            allValid = false;
            continue;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceNormalIndices[start + j];
            if (idx < 0 || idx >= maxNormal) {
                buildLogErrorAndPrint(ParseError::FaceIndexOutOfBounds);
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
        buildLogErrorAndPrint(ParseError::EmptyData);
        return false;
    }

    if (polygonStarts.size() != polygonLengths.size()) {
        buildLogErrorAndPrint(ParseError::CorruptedDataStructure);
        return false;
    }

    int totalIndices = 0;
    int maxVertex = vertexData.size();

    for (int i = 0; i < polygonLengths.size(); ++i) {
        int start = polygonStarts[i];
        int count = polygonLengths[i];

        if (count < 3) {
            buildLogErrorAndPrint(ParseError::FaceLessThanThreeVertices);
            return false;
        }

        for (int j = 0; j < count; ++j) {
            int idx = faceVertexIndices[start + j];
            if (idx < 0 || idx >= maxVertex) {
                buildLogErrorAndPrint(ParseError::FaceIndexOutOfBounds);
                return false;
            }
        }

        totalIndices += count;
    }

    if (totalIndices != faceVertexIndices.size()) {
        buildLogErrorAndPrint(ParseError::CorruptedDataStructure);
        return false;
    }

    return true;
}

bool validateModel(const Model &model)
{
    return checkVertices(model.vertices) &&
           checkTexCoords(model.textureVertices, model.faceTexCoordIndices, model.polygonStarts, model.polygonLengths) &&
           checkNormals(model.normals, model.faceNormalIndices, model.polygonStarts, model.polygonLengths) &&
           checkFaces(model.vertices, model.faceVertexIndices, model.polygonStarts, model.polygonLengths);
}

bool parseTokens(QTextStream &in, Model &model)
{
    int lineNum = 0;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        ++lineNum;

        if (line.isEmpty())
            continue;

        qDebug() << "[DEBUG] Строка" << lineNum << ":" << line;

        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.isEmpty()) {
            qDebug() << "  [SKIP] Пустая строка или только пробелы";
            continue;
        }

        qDebug() << "  Токены:" << tokens;

        const QString type = tokens[0];

        if (type == "#") {
            qDebug() << "  [COMMENT] Пропущено.";
            continue;
        }

        if (type == "v") {
            qDebug() << "  [VERTEX] Обработка вершины...";
            if (!parseVertex(tokens, line, model.vertices, lineNum)) {
                qDebug() << "  [ERROR] Ошибка при обработке вершины на строке" << lineNum;
                return false;
            }
            qDebug() << "  [SUCCESS] Вершина добавлена. Текущее количество вершин:" << model.vertices.size();
        }

        else if (type == "vt") {
            qDebug() << "  [TEXCOORD] Обработка текстурной координаты...";
            if (!parseTexCoord(tokens, line, model.textureVertices, lineNum)) {
                qDebug() << "  [ERROR] Ошибка при обработке текстурной координаты на строке" << lineNum;
                return false;
            }
            qDebug() << "  [SUCCESS] Текстурная координата добавлена. Количество:" << model.textureVertices.size();
        }

        else if (type == "vn") {
            qDebug() << "  [NORMAL] Обработка нормали...";
            if (!parseNormal(tokens, line, model.normals, lineNum)) {
                qDebug() << "  [ERROR] Ошибка при обработке нормали на строке" << lineNum;
                return false;
            }
            qDebug() << "  [SUCCESS] Нормаль добавлена. Количество:" << model.normals.size();
        }

        else if (type == "f") {
            qDebug() << "  [FACE] Обработка полигона...";
            if (!parseFace(tokens, line,
                           model.faceVertexIndices,
                           model.faceTexCoordIndices,
                           model.faceNormalIndices,
                           model.polygonStarts,
                           model.polygonLengths,
                           lineNum))
            {
                qDebug() << "  [ERROR] Ошибка при обработке полигона на строке" << lineNum;
                return false;
            }
            qDebug() << "  [SUCCESS] Полигон добавлен.";
            qDebug() << "    Индексы вершин:" << model.faceVertexIndices;
            qDebug() << "    Индексы текстур:" << model.faceTexCoordIndices;
            qDebug() << "    Индексы нормалей:" << model.faceNormalIndices;
            qDebug() << "    polygonStarts:" << model.polygonStarts;
            qDebug() << "    polygonLengths:" << model.polygonLengths;
        }

        else {
            qDebug() << "  [WARNING] Неизвестный тип строки:" << type;
        }

        qDebug() << "--------------------------------------------------";
    }

    qDebug() << "[INFO] Парсинг завершён успешно.";
    qDebug() << "  Вершин всего:" << model.vertices.size();
    qDebug() << "  Текстурных координат:" << model.textureVertices.size();
    qDebug() << "  Нормалей:" << model.normals.size();
    qDebug() << "  Индексы вершин полигонов:" << model.faceVertexIndices.size();
    qDebug() << "  Индексы текстур полигонов:" << model.faceTexCoordIndices.size();
    qDebug() << "  Индексы нормалей полигонов:" << model.faceNormalIndices.size();

    return true;
}

bool parseObj(const QString &filePath, Model &model)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        buildLogErrorAndPrint(ParseError::FileOpenFailed);
        return false;
    }

    QTextStream in(&file);
    return parseTokens(in, model);
}

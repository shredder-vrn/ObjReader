#include "objreader.h"
#include "meshdata.h"

#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

namespace{

bool parseVertexLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum){

    if (tokens.size() < 4){
        qWarning() << "Недостаточно для вершины (строка: " << lineNum << "): " << line;
        return false;
    }

    bool okX = false;
    bool okY = false;
    bool okZ = false;

    float x = tokens[1].toFloat(&okX);
    float y = tokens[2].toFloat(&okY);
    float z = tokens[3].toFloat(&okZ);

    if (!okX || !okY || !okZ){
        qWarning() << "Ошибка парсинга вершины: (строка: " << lineNum << "): " << line
                    << "x:" << tokens[1] << (okX ? "OK" : "Ошибка")
                    << "y:" << tokens[2] << (okY ? "OK" : "Ошибка")
                    << "z:" << tokens[3] << (okZ ? "OK" : "Ошибка");
        return false;
    }

    model.vertices.append(QVector3D(x, y, z));
    return true;
}

bool parseTexCoordLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum){

    if (tokens.size() < 3){
        qWarning() << "Недостаточно для текстурной координаты (строка: " << lineNum << "): " << line;
        return false;
    }

    bool okU = false;
    bool okV = false;

    float u = tokens[1].toFloat(&okU);
    float v = tokens[2].toFloat(&okV);

    if (!okU || !okV){
        qWarning() << "Ошибка парсинга вершины: (строка: " << lineNum << "): " << line
                    << "u:" << tokens[1] << (okU ? "OK" : "Ошибка")
                    << "v:" << tokens[2] << (okV ? "OK" : "Ошибка");
        return false;
    }

    model.texCoords.append(QVector2D(u, v));
    return true;
}

bool parseNormalLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum){

    if (tokens.size() < 4){
        qWarning() << "Недостаточно для нормали (строка: " << lineNum << "): " << line;
        return false;
    }

    bool okX = false;
    bool okY = false;
    bool okZ = false;

    float x = tokens[1].toFloat(&okX);
    float y = tokens[2].toFloat(&okY);
    float z = tokens[3].toFloat(&okZ);

    if (!okX || !okY || !okZ){
        qWarning() << "Ошибка парсинга нормали: (строка: " << lineNum << "): " << line
                    << "x:" << tokens[1] << (okX ? "OK" : "Ошибка")
                    << "y:" << tokens[2] << (okY ? "OK" : "Ошибка")
                    << "z:" << tokens[3] << (okZ ? "OK" : "Ошибка");
        return false;
    }

    model.normals.append(QVector3D(x, y, z));
    return true;
}

bool parseFaceLine(const QStringList &tokens, Model &model, const QString &line, int &lineNum){

    Face face;

    for (int i = 1; i < tokens.size(); ++i){
        QStringList parts = tokens[i].split('/');

        if (parts.size() >= 1){
            bool ok = false;
            int vertexIndex = parts[0].toInt(&ok);

            if (!ok) {
                qWarning() << "Индекс вершины не число: (строка: " << lineNum << "): " << line << parts[0];
                return false;
            }
            if (vertexIndex > 0) {
                face.vertexIndices.append(vertexIndex-1);
            } else {
                qWarning() << "Индекс вершины <= 0: (строка: " << lineNum << "): " << line << " --> " << parts[0];
                return false;
            }
        } else {
            qWarning() << "Недостаточно данных для вершины в полигоне  (строка: " << lineNum << "): " << line;
            return false;
        }

        if (parts.size() >= 2 && !parts[1].isEmpty()){
            bool ok = false;
            int texCoordIndex = parts[1].toInt(&ok);
            if (!ok) {
                qWarning() << "Индекс текстурной координаты не число:  (строка: " << lineNum << "): " << line << parts[1];
                face.texCoordIndices.append(-1);
            } else if (texCoordIndex > 0) {
                face.texCoordIndices.append(texCoordIndex-1);
            } else {
                qWarning() << "Индекс текстурной координаты <= 0: (строка: " << lineNum << "): " << line << parts[0];
                face.texCoordIndices.append(-1);
            }
        } else {
            qWarning() << "Недостаточно данных для текстурной координаты в полигоне (строка: " << lineNum << "): " << line;
            face.texCoordIndices.append(-1);
        }

        if (parts.size() >= 3 && !parts[2].isEmpty()){
            bool ok = false;
            int normalIndex = parts[2].toInt(&ok);
            if (!ok) {
                qWarning() << "Индекс нормали не число: (строка: " << lineNum << "): " << line << parts[1];
                face.normalIndices.append(-1);
            } else if (normalIndex > 0) {
                face.normalIndices.append(normalIndex-1);
            } else {
                qWarning() << "Индекс нормали <= 0: (строка: " << lineNum << "): " << line << parts[0];
                face.normalIndices.append(-1);
            }
        } else {
            qWarning() << "Недостаточно данных для нормали в полигоне (строка: " << lineNum << "): " << line;
            face.normalIndices.append(-1);
        }
    }

    if (face.vertexIndices.size() < 3){
        qWarning() << "Грань содержит меньше 3-х вершин  (строка: " << lineNum << "): " << line;
        return false;
    }

    model.faces.append(face);
    return true;
}

}


bool parseObjvertices(const QString &filePath, Model &model)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Не удалось открыть файл" << filePath;
        return false;
    }

    QTextStream in(&file);

    int lineNum = 0;

    while (!in.atEnd()){
        QString line = in.readLine().trimmed();
        ++lineNum;

        if (line.isEmpty())
            continue;

        QStringList tokens = line.split(' ', Qt::SkipEmptyParts);

        if (tokens.isEmpty())
            continue;

        const QString type = tokens[0];

        if (type == "#"){
            continue;
        } else if (type == "v") {
            parseVertexLine(tokens, model, line, lineNum);
        } else if (type == "vt") {
            parseTexCoordLine(tokens, model, line, lineNum);
        } else if (type == "vn") {
            parseNormalLine(tokens, model, line, lineNum);
        } else if (type == "f") {
            parseFaceLine(tokens, model, line, lineNum);
        }
    }
    return true;
}

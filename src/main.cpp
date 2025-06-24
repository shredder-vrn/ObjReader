#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>


#include "../src/obj_reader/modeldata.h"
#include "../src/obj_reader/objreader.h"


void printStats(const ModelV2 &model)
{
    qDebug() << "|----------------------------------------------|";
    qDebug() << "Вершин: " << model.vertexData.size() / 3;
    qDebug() << "Текстурных координат: " << model.texCoordData.size() / 2;
    qDebug() << "Нормалей: " << model.normalData.size() / 3;
    qDebug() << "Полигонов: " << model.polygonLengths.size();
    qDebug() << "|----------------------------------------------|";

    for (int i = 0; i < model.vertexData.size(); i += 3) {
        QVector3D v(model.vertexData[i], model.vertexData[i+1], model.vertexData[i+2]);
        qDebug() << "v" << (i / 3 + 1) << ":" << v;
    }

    for (int i = 0; i < model.texCoordData.size(); i += 2) {
        QVector2D vt(model.texCoordData[i], model.texCoordData[i+1]);
        qDebug() << "vt" << (i / 2 + 1) << ":" << vt;
    }

    for (int i = 0; i < model.normalData.size(); i += 3) {
        QVector3D vn(model.normalData[i], model.normalData[i+1], model.normalData[i+2]);
        qDebug() << "vn" << (i / 3 + 1) << ":" << vn;
    }

    for (int i = 0; i < model.polygonLengths.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = model.polygonLengths[i];

        QVector<int> vertexIndices;
        QVector<int> texCoordIndices;
        QVector<int> normalIndices;

        for (int j = 0; j < count; ++j) {
            if (start + j < model.faceVertexIndices.size())
                vertexIndices << model.faceVertexIndices[start + j];
            if (start + j < model.faceTexCoordIndices.size())
                texCoordIndices << model.faceTexCoordIndices[start + j];
            if (start + j < model.faceNormalIndices.size())
                normalIndices << model.faceNormalIndices[start + j];
        }

        qDebug() << "f" << i+1 << ":"
                 << "v" << vertexIndices
                 << "vt" << texCoordIndices
                 << "vn" << normalIndices;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    ModelV2 model;

    QString filename = "/home/r3ds/Internship/resources/cube.obj";

    QFile file(filename);
    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filename;
        return 1;
    }

    if (!parseObj(filename, model)) {
        qCritical() << "Ошибка при чтении файла";
        return 1;
    }

    if (!validateModel(model)) {
        qCritical() << "Модель невалидна!";
        return 1;
    }

    qDebug() << "Модель успешно загружена и валидна.";
    printStats(model);

    return 0;
}

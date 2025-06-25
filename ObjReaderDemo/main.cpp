#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>


#include "ObjReader/objreader.h"
#include "Model/model.h"


void printStats(const Model &model)
{
    qDebug() << "|----------------------------------------------|";
    qDebug() << "Вершин (v):" << model.vertices.size();
    qDebug() << "Текстурных координат (vt):" << model.textureVertices.size();
    qDebug() << "Нормалей (vn):" << model.normals.size();
    qDebug() << "Полигонов (f):" << model.polygonLengths.size();
    qDebug() << "|----------------------------------------------|";

    if (!model.vertices.isEmpty()) qDebug() << "Вершины:";
    for (int i = 0; i < model.vertices.size(); ++i) {
        qDebug() << "v" << (i + 1) << ":" << model.vertices[i];
    }

    if (!model.textureVertices.isEmpty()) qDebug() << "Текстурные координаты:";
    for (int i = 0; i < model.textureVertices.size(); ++i) {
        qDebug() << "vt" << (i + 1) << ":" << model.textureVertices[i];
    }

    if (!model.normals.isEmpty()) qDebug() << "Нормали:";
    for (int i = 0; i < model.normals.size(); ++i) {
        qDebug() << "vn" << (i + 1) << ":" << model.normals[i];
    }

    if (!model.polygonLengths.isEmpty()) qDebug() << "Полигоны:";
    for (int i = 0; i < model.polygonLengths.size(); ++i) {
        int start = model.polygonStarts[i];
        int count = model.polygonLengths[i];

        QStringList faceParts;

        for (int j = 0; j < count; ++j) {
            QString part = QString::number(model.faceVertexIndices[start + j] + 1);

            if (!model.faceTexCoordIndices.isEmpty() && start + j < model.faceTexCoordIndices.size())
                part += "/" + QString::number(model.faceTexCoordIndices[start + j] + 1);

            if (!model.faceNormalIndices.isEmpty() && start + j < model.faceNormalIndices.size())
                part += "/" + QString::number(model.faceNormalIndices[start + j] + 1);

            faceParts << part;
        }

        qDebug() << "f" << (i + 1) << ":" << faceParts.join(" ");
    }

    qDebug() << "|----------------------------------------------|";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Model model;

    QString filename = "/home/r3ds/Internship/resources/cube.obj";

    QFile file(filename);
    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filename;
        return 1;
    }

    // Открываем файл и читаем содержимое
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл для чтения:" << filename;
        return 1;
    }

    QTextStream in(&file);
    if (!parseTokens(in, model)) {
        qCritical() << "Ошибка при разборе файла OBJ.";
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

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

void launchReader(const QString &filename){
    Model model;

    QFile file(filename);
    if (!file.exists()) {
        qCritical() << "Файл не найден:" << filename;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Не удалось открыть файл для чтения:" << filename;
    }

    QTextStream in(&file);
    if (!parseTokens(in, model)) {
        qCritical() << "Ошибка при разборе файла OBJ.";
    }

    printStats(model);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QString filename = "/home/r3ds/Internship/resources/HARDCORE/cube.obj";
    const bool launchOk = true;

    if (launchOk){
        launchReader(filename);
    }
    return 0;
}



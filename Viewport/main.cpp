#include <QCoreApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>
#include <QApplication>


#include "ObjReader/objreader.h"
#include "Model/model.h"
#include "viewport.h"


void printStats(const Model &model)
{
    qDebug() << "|----------------------------------------------|";
    qDebug() << "Вершин (v):" << model.vertices.size();
    qDebug() << "Текстурных координат (vt):" << model.textureVertices.size();
    qDebug() << "Нормалей (vn):" << model.normals.size();
    qDebug() << "Полигонов (f):" << model.polygonStarts.size();
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

    if (!model.polygonStarts.isEmpty()) qDebug() << "Полигоны:";

    for (int i = 0; i < model.polygonStarts.size(); ++i) {
        int start = model.polygonStarts[i];
        int next = (i < model.polygonStarts.size() - 1) ? model.polygonStarts[i + 1] : model.faceVertexIndices.size();
        int count = next - start;

        QStringList faceParts;

        for (int j = 0; j < count; ++j) {
            QString part = QString::number(model.faceVertexIndices[start + j] + 1);

            if (!model.faceTextureVertexIndices.isEmpty() && (start + j < model.faceTextureVertexIndices.size())) {
                int ti = model.faceTextureVertexIndices[start + j];
                if (ti != -1) {
                    part += "/" + QString::number(ti + 1);
                } else {
                    part += "/";
                }
            }

            if (!model.faceNormalIndices.isEmpty() && (start + j < model.faceNormalIndices.size())) {
                int ni = model.faceNormalIndices[start + j];
                if (ni != -1) {
                    part += "/" + QString::number(ni + 1);
                }
            }

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
    QApplication app(argc, argv);

    QString filename = "/home/r3ds/Internship/resources/cube.obj";
    const bool launchOk = false;

    if (launchOk){
        launchReader(filename);
    }


    Viewport viewport;
    viewport.resize(800, 600);
    viewport.show();

    return app.exec();
}




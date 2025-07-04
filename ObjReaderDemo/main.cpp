#include <QApplication>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>

#include "ObjReader/objreader.h"
#include <Viewport/viewport.h>
#include "Model/model.h"



void printStats(const Model &model)
{
    qDebug() << "|----------------------------------------------|";
    qDebug() << "Вершин (v):" << model.m_vertices.size();
    qDebug() << "Текстурных координат (vt):" << model.m_textureVertices.size();
    qDebug() << "Нормалей (vn):" << model.m_normals.size();
    qDebug() << "Полигонов (f):" << model.m_polygonStarts.size();
    qDebug() << "|----------------------------------------------|";

    if (!model.m_vertices.isEmpty()) qDebug() << "Вершины:";
    for (int i = 0; i < model.m_vertices.size(); ++i) {
        qDebug() << "v" << (i + 1) << ":" << model.m_vertices[i];
    }

    if (!model.m_textureVertices.isEmpty()) qDebug() << "Текстурные координаты:";
    for (int i = 0; i < model.m_textureVertices.size(); ++i) {
        qDebug() << "vt" << (i + 1) << ":" << model.m_textureVertices[i];
    }

    if (!model.m_normals.isEmpty()) qDebug() << "Нормали:";
    for (int i = 0; i < model.m_normals.size(); ++i) {
        qDebug() << "vn" << (i + 1) << ":" << model.m_normals[i];
    }

    if (!model.m_polygonStarts.isEmpty()) qDebug() << "Полигоны:";

    for (int i = 0; i < model.m_polygonStarts.size(); ++i) {
        int start = model.m_polygonStarts[i];
        int next = (i < model.m_polygonStarts.size() - 1) ? model.m_polygonStarts[i + 1] : model.m_faceVertexIndices.size();
        int count = next - start;

        QStringList faceParts;

        for (int j = 0; j < count; ++j) {
            QString part = QString::number(model.m_faceVertexIndices[start + j] + 1);

            if (!model.m_faceTextureVertexIndices.isEmpty() && (start + j < model.m_faceTextureVertexIndices.size())) {
                int ti = model.m_faceTextureVertexIndices[start + j];
                if (ti != -1) {
                    part += "/" + QString::number(ti + 1);
                } else {
                    part += "/";
                }
            }

            if (!model.m_faceNormalIndices.isEmpty() && (start + j < model.m_faceNormalIndices.size())) {
                int ni = model.m_faceNormalIndices[start + j];
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
    const bool launchOk = true;

    if (launchOk){
        launchReader(filename);
    }


    Viewport viewport;
    viewport.resize(800, 600);
    viewport.show();

    return app.exec();
}



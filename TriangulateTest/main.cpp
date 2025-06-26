#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtGlobal>
#include <QCoreApplication>
#include <QtTest/QtTest>
#include "Triangulate/triangulate.h"
#include "Model/model.h"
#include "ObjReader/objreader.h"
#include <Viewport/viewport.h>

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

    simpleTriangulateModel(
        model.vertices,
        model.faceVertexIndices,
        model.faceTextureVertexIndices,
        model.faceNormalIndices,
        model.polygonStarts
    );

}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QString filename = "/home/r3ds/Internship/resources/cube.obj";
    const bool launchOk = true;
    if (launchOk)
        launchReader(filename);
    return 0;
}

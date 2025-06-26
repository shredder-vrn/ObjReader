#include <QApplication>

#include "viewport.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Viewport viewport;
    viewport.resize(800, 600);
    QString filePath = "/home/r3ds/Internship/resources/cube.obj";
    viewport.loadModel(filePath);
    viewport.show();

    return app.exec();
}




#include <QApplication>

#include "MainWindow/mainwindow.h"

int main(int argc, char *argv[])
{
    qDebug() << "main : запустили main";
    QApplication app(argc, argv);
    Viewer::MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}




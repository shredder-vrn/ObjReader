#include <QApplication>

#include "viewport.h"

#include "MainWindow/mainwindow.h"
#include "MainWindow/mainwindowrefactoring.h"

int main(int argc, char *argv[])
{
    qDebug() << "main : запустили main";
    QApplication app(argc, argv);
    Viewer::MainWindow mainWindow;
    //MainWindowRefactoring mainWindowRefactoring;
    mainWindow.show();
    return app.exec();
}




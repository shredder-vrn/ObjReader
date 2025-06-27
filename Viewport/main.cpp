#include <QApplication>

#include "viewport.h"
<<<<<<< Updated upstream

#include <QApplication>
#include "MainWindow/mainwinndow.h"

int main(int argc, char *argv[]) {
=======
#include "MainWindow/mainwindow.h"

int main(int argc, char *argv[])
{
    qDebug() << "main : запустили main";

>>>>>>> Stashed changes
    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.show();

    return app.exec();

    qDebug() << "main : main отработал";

}



#include <QApplication>

#include "viewport.h"

#include <QApplication>
#include "MainWindow/mainwinndow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}



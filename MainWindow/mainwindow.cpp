#include "mainwinndow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSizePolicy>

#include "Viewport/viewport.h"

MainWindow :: MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qDebug() << "MainWindow :: MainWindow : запустили конструктор";

    setWindowTitle("OBJ Wiewer");

    resize(800, 600);

    QWidget *centralWidget = new QWidget(this);

    QVBoxLayout *layout = new QVBoxLayout();

    centralWidget -> setLayout(layout);

    viewport = new Viewport(centralWidget);

    layout -> addWidget(viewport, 0, Qt::Alignment());

    qDebug() << "MainWindow :: addWidget : почему я меняю параметры addWidget, но ничего не меняется?.";

    setCentralWidget(centralWidget);

    QMenu *fileMenu = menuBar() -> addMenu("File");

    const QAction *openAction = fileMenu -> addAction("Open");

    connect(openAction, &QAction::triggered, this, &MainWindow::openModel);

    qDebug() << "MainWindow :: MainWindow : конструктор отработал, все преобразования обработаны.";
}

void MainWindow::openModel()
{
    qDebug() << "MainWindow :: openModel : запустился метод openModel";

    //QString filePath = QFileDialog ::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));

    viewport -> loadModel("/home/r3ds/Internship/resources/cube.obj");

    qDebug() << "MainWindow :: openModel : метод openModel отработал, все преобразования обработаны.";

}

#include "mainwinndow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include "Viewport/viewport.h"

MainWindow :: MainWindow(QWidget *parent) : QMainWindow (parent)
{
    setWindowTitle("OBJ Wiewer");
    resize(800, 600);

    QWidget *centralWidget = new QWidget (this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    viewport = new Viewport(centralWidget);
    layout->addWidget(viewport);

    setCentralWidget(centralWidget);

    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QAction* openAction = fileMenu->addAction(tr("&Open..."));
    connect(openAction, &QAction::triggered, this, &MainWindow::openModel);
}

void MainWindow::openModel() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));

    if (!filePath.isEmpty())
        viewport ->loadModel(filePath);
}

MainWindow::~MainWindow() = default;

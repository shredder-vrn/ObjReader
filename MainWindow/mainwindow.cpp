#include "mainwindow.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFormLayout>
#include <QLabel>
#include <QFileSystemModel>
#include <QGroupBox>

#include "Viewport/viewport.h"

namespace Viewer
{
MainWindow :: MainWindow(QWidget *parent) : QMainWindow(parent)
{
    qDebug() << "MainWindow :: MainWindow : запустили конструктор";

    setWindowTitle("OBJ Wiewer");
    resize(1400, 900);

    QWidget *centralWidget = new QWidget(this);

    m_viewport = new ViewportWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout();

    layout->addWidget(m_viewport);
    centralWidget -> setLayout(layout);

    setCentralWidget(centralWidget);

    setupDockWidgets();

    QMenu *fileMenu = menuBar() -> addMenu("File");

    const QAction *openAction = fileMenu -> addAction("Open");

    connect(openAction, &QAction::triggered, this, &MainWindow::openModel);
}

void MainWindow::setupDockWidgets()
{
    m_explorerDock = new QDockWidget("Explorer", this);
    m_explorerView = new QTreeView(m_explorerDock);
    m_explorerDock->setWidget(m_explorerView);

    addDockWidget(Qt::LeftDockWidgetArea, m_explorerDock);

    m_propertiesDock = new QDockWidget("Properties", this);

    QWidget *propertiesPanel = new QWidget(m_propertiesDock);
    QVBoxLayout *mainLayout = new QVBoxLayout(propertiesPanel);

    mainLayout->addWidget(createModelPropertiesSection());
    mainLayout->addWidget(createRenderOptionsSection());
    mainLayout->addWidget(createTransformSection());
    mainLayout->addStretch();

    propertiesPanel->setLayout(mainLayout);

    m_propertiesDock->setWidget(propertiesPanel);
    m_propertiesDock->setMinimumWidth(350);

    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    QMenu *cameraMenu = menuBar()->addMenu("Camera");

    QAction *perspAct = cameraMenu->addAction("Perspective");
    QAction *orthoAct = cameraMenu->addAction("Orthographic");

    connect(perspAct, &QAction::triggered, m_viewport, &ViewportWidget::switchToPerspective);
    connect(orthoAct, &QAction::triggered, m_viewport, &ViewportWidget::switchToOrthographic);
}

QGroupBox *MainWindow::createModelPropertiesSection()
{
    QGroupBox *group = new QGroupBox("Properties");
    QFormLayout *layout = new QFormLayout();

    m_modelNameLabel = new QLabel("None");
    m_verticesLabel = new QLabel("0");
    m_facesLabel = new QLabel("0");

    layout->addRow("Name:", m_modelNameLabel);
    layout->addRow("Vertices:", m_verticesLabel);
    layout->addRow("Faces:", m_facesLabel);

    group->setLayout(layout);
    return group;
}

QGroupBox *MainWindow::createRenderOptionsSection()
{
    QGroupBox *group = new QGroupBox("Render Options");
    QVBoxLayout *layout = new QVBoxLayout();

    m_wireframeCheck = new QCheckBox("Wireframe");
    m_textureCheck = new QCheckBox("Textures");
    m_lightingCheck = new QCheckBox("Lighting");
    m_normalsCheck = new QCheckBox("Show Normals");

    layout->addWidget(m_wireframeCheck);
    layout->addWidget(m_textureCheck);
    layout->addWidget(m_lightingCheck);
    layout->addWidget(m_normalsCheck);

    connect(m_wireframeCheck, &QCheckBox::stateChanged, this, &MainWindow::updateRenderOptions);
    connect(m_textureCheck, &QCheckBox::stateChanged, this, &MainWindow::updateRenderOptions);
    connect(m_lightingCheck, &QCheckBox::stateChanged, this, &MainWindow::updateRenderOptions);
    connect(m_normalsCheck, &QCheckBox::stateChanged, this, &MainWindow::updateRenderOptions);

    group->setLayout(layout);
    return group;
}

QGroupBox* MainWindow::createTransformSection()
{
    QGroupBox *group = new QGroupBox("Transform");
    QFormLayout *layout = new QFormLayout();

    auto initSpinBox = [this](double min, double max, double value) {
        QDoubleSpinBox *spin = new QDoubleSpinBox(this);
        spin->setRange(min, max);
        spin->setValue(value);
        spin->setSingleStep(0.1);
        spin->setDecimals(3);
        connect(spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, &MainWindow::updateModelTransform);
        return spin;
    };

    m_posXSpin = initSpinBox(-1000, 1000, 0);
    m_posYSpin = initSpinBox(-1000, 1000, 0);
    m_posZSpin = initSpinBox(-1000, 1000, 0);

    m_rotXSpin = initSpinBox(-360, 360, 0);
    m_rotYSpin = initSpinBox(-360, 360, 0);
    m_rotZSpin = initSpinBox(-360, 360, 0);

    m_scaleXSpin = initSpinBox(0.01, 10, 1);
    m_scaleYSpin = initSpinBox(0.01, 10, 1);
    m_scaleZSpin = initSpinBox(0.01, 10, 1);

    layout->addRow("Position X:", m_posXSpin);
    layout->addRow("Position Y:", m_posYSpin);
    layout->addRow("Position Z:", m_posZSpin);

    layout->addRow("Rotation X:", m_rotXSpin);
    layout->addRow("Rotation Y:", m_rotYSpin);
    layout->addRow("Rotation Z:", m_rotZSpin);

    layout->addRow("Scale X:", m_scaleXSpin);
    layout->addRow("Scale Y:", m_scaleYSpin);
    layout->addRow("Scale Z:", m_scaleZSpin);

    group->setLayout(layout);
    return group;
}

void MainWindow::updateRenderOptions()
{

}

void MainWindow::updateModelTransform()
{

}



void MainWindow::openModel()
{
    qDebug() << "MainWindow :: openModel : запустился метод openModel";
    //QString filePath = QFileDialog ::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));
    m_viewport -> loadModel("/home/r3ds/Internship/resources/cube.obj");
}

}

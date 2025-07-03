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
#include <QPushButton>

#include "Viewport/viewport.h"

namespace Viewer
{
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    qDebug() << "MainWindow :: MainWindow : запустили конструктор";

    setWindowTitle("OBJ Viewer");
    resize(1400, 900);

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_viewport = new ViewportWidget(this);
    layout->addWidget(m_viewport);

    setCentralWidget(centralWidget);
    setupDockWidgets();

    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open");
    connect(openAction, &QAction::triggered, this, &MainWindow::openModel);
}

void MainWindow::setupDockWidgets()
{
    m_explorerDock = new QDockWidget("Explorer", this);
    m_explorerView = new QTreeView(m_explorerDock);
    m_explorerModel = new QStandardItemModel(this);
    m_explorerModel->setHorizontalHeaderLabels({"Models"});
    m_explorerView->setModel(m_explorerModel);
    m_explorerDock->setWidget(m_explorerView);
    addDockWidget(Qt::LeftDockWidgetArea, m_explorerDock);

    connect(m_explorerView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &MainWindow::onModelSelected);

    m_propertiesDock = new QDockWidget("Properties", this);
    QWidget* propertiesPanel = new QWidget(m_propertiesDock);
    QVBoxLayout* mainLayout = new QVBoxLayout(propertiesPanel);

    mainLayout->addWidget(createModelPropertiesSection());
    mainLayout->addWidget(createRenderOptionsSection());
    mainLayout->addWidget(createTransformSection());
    mainLayout->addStretch();

    propertiesPanel->setLayout(mainLayout);
    m_propertiesDock->setWidget(propertiesPanel);
    m_propertiesDock->setMinimumWidth(350);
    addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

    QMenu* cameraMenu = menuBar()->addMenu("Camera");
    QAction* perspAct = cameraMenu->addAction("Perspective");
    QAction* orthoAct = cameraMenu->addAction("Orthographic");
    connect(perspAct, &QAction::triggered, m_viewport, &ViewportWidget::switchToPerspective);
    connect(orthoAct, &QAction::triggered, m_viewport, &ViewportWidget::switchToOrthographic);
}

QGroupBox* MainWindow::createModelPropertiesSection()
{
    QGroupBox* group = new QGroupBox("Properties");
    QFormLayout* layout = new QFormLayout();

    m_modelNameLabel = new QLabel("None");
    m_verticesLabel = new QLabel("0");
    m_facesLabel = new QLabel("0");

    layout->addRow("Name:", m_modelNameLabel);
    layout->addRow("Vertices:", m_verticesLabel);
    layout->addRow("Faces:", m_facesLabel);

    group->setLayout(layout);
    return group;
}

QGroupBox* MainWindow::createRenderOptionsSection()
{
    QGroupBox* group = new QGroupBox("Render Options");
    QVBoxLayout* layout = new QVBoxLayout();

    m_wireframeCheck = new QCheckBox("Wireframe");
    m_textureCheck = new QCheckBox("Use Texture");
    m_lightingCheck = new QCheckBox("Lighting");
    m_normalsCheck = new QCheckBox("Show Normals");

    layout->addWidget(m_wireframeCheck);
    layout->addWidget(m_textureCheck);
    layout->addWidget(m_lightingCheck);
    layout->addWidget(m_normalsCheck);

    connect(m_textureCheck, &QCheckBox::toggled, this, &MainWindow::updateSelectedModelTextureState);

    group->setLayout(layout);
    return group;
}

void MainWindow::updateSelectedModelTextureState(bool checked)
{
    if (m_selectedModelIndex >= 0 && m_selectedModelIndex < m_models.size()) {
        m_models[m_selectedModelIndex].hasTexture = checked;
        m_viewport->setModels(m_models, m_modelTransforms);
    }
}

void MainWindow::onTextureCheckToggled(bool checked)
{
    for (auto& model : m_models) {
        model.hasTexture = checked;
    }

    m_viewport->setModels(m_models, m_modelTransforms);
}

QGroupBox* MainWindow::createTransformSection()
{
    QGroupBox* group = new QGroupBox("Transform");
    QFormLayout* layout = new QFormLayout();

    auto initSpinBox = [this](double min, double max, double value) {
        QDoubleSpinBox* spin = new QDoubleSpinBox(this);
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

    m_loadTextureButton = new QPushButton("Load Texture", this);
    layout->addRow(m_loadTextureButton);

    connect(m_loadTextureButton, &QPushButton::clicked, this, &MainWindow::loadTextureForSelectedModel);

    group->setLayout(layout);
    return group;
}

void MainWindow::openModel()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));
    if (filePath.isEmpty())
        return;

    Model model;
    ModelController controller;
    if (!controller.loadModel(filePath)) {
        qWarning() << "Не удалось загрузить модель";
        return;
    }

    m_models.append(controller.getModel());
    m_modelTransforms.append(QMatrix4x4());

    updateModelList();
    m_viewport->setModels(m_models, m_modelTransforms);
}

void MainWindow::updateModelList()
{
    m_explorerModel->removeRows(0, m_explorerModel->rowCount());
    for (int i = 0; i < m_models.size(); ++i) {
        QStandardItem* item = new QStandardItem(QString("Model %1").arg(i + 1));
        m_explorerModel->appendRow(item);
    }
}

void MainWindow::onModelSelected(const QModelIndex& index)
{
    m_selectedModelIndex = index.row();
    if (m_selectedModelIndex >= 0 && m_selectedModelIndex < m_models.size()) {
        const Model& model = m_models[m_selectedModelIndex];
        m_modelNameLabel->setText(QString("Model %1").arg(m_selectedModelIndex + 1));
        m_verticesLabel->setText(QString::number(model.vertices.size()));
        m_facesLabel->setText(QString::number(model.faceVertexIndices.size() / 3));

        m_textureCheck->setChecked(model.hasTexture);

        qDebug() << "[DEBUG] Выбрана модель #" << m_selectedModelIndex;
        qDebug() << "hasTexture:" << model.hasTexture;
        qDebug() << "textureId:" << model.textureId;
    }

}

void MainWindow::updateModelTransform()
{
    if (m_selectedModelIndex < 0 || m_selectedModelIndex >= m_models.size())
        return;

    float px = m_posXSpin->value();
    float py = m_posYSpin->value();
    float pz = m_posZSpin->value();

    float rx = m_rotXSpin->value();
    float ry = m_rotYSpin->value();
    float rz = m_rotZSpin->value();

    float sx = m_scaleXSpin->value();
    float sy = m_scaleYSpin->value();
    float sz = m_scaleZSpin->value();

    QMatrix4x4 transform;
    transform.translate(px, py, pz);
    transform.rotate(rx, 1, 0, 0);
    transform.rotate(ry, 0, 1, 0);
    transform.rotate(rz, 0, 0, 1);
    transform.scale(sx, sy, sz);

    m_modelTransforms[m_selectedModelIndex] = transform;
    m_viewport->setModels(m_models, m_modelTransforms);
}

void MainWindow::loadTextureForSelectedModel()
{
    if (m_selectedModelIndex < 0 || m_selectedModelIndex >= m_models.size())
        return;

    QString texturePath = QFileDialog::getOpenFileName(this, tr("Open Texture"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (texturePath.isEmpty())
        return;

    if (!m_viewport->loadTextureForModel(texturePath, m_selectedModelIndex)) {
        qWarning() << "Не удалось загрузить текстуру";
        return;
    }

    m_models[m_selectedModelIndex].hasTexture = true;
    m_textureCheck->setChecked(true);
    m_viewport->setModels(m_models, m_modelTransforms);
}
}

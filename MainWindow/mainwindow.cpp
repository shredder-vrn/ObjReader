#include "mainwindow.h"

namespace Viewer
{
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    qDebug() << "MainWindow::MainWindow : запустили конструктор";
    setWindowTitle("OBJ Viewer");
    resize(1400, 900);    

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_viewport = new ViewportWidget(this);
    layout->addWidget(m_viewport);

    setCentralWidget(centralWidget);
    setupUserInterface();

    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open");

    connect(openAction, &QAction::triggered, this, &MainWindow::openModelFile);
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow::~MainWindow : запустили деструктор";
    for (auto* model : m_models) {delete model;}

    m_models.clear();
}

void MainWindow::setupUserInterface()
{
    qDebug() << "MainWindow :: setupUserInterface : запустили конструктор";
    m_explorerDock = new QDockWidget("Explorer", this);
    m_explorerView = new QTreeView(m_explorerDock);
    m_explorerModel = new QStandardItemModel(this);

    m_explorerModel->setHorizontalHeaderLabels({"Models"});
    m_explorerView->setModel(m_explorerModel);
    m_explorerDock->setWidget(m_explorerView);
    addDockWidget(Qt::LeftDockWidgetArea, m_explorerDock);

    connect(m_explorerView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this, &MainWindow::onExplorerModelSelected);

    m_propertiesDock = new QDockWidget("Properties", this);
    QWidget* propertiesPanel = new QWidget(m_propertiesDock);
    QVBoxLayout* mainLayout = new QVBoxLayout(propertiesPanel);

    mainLayout->addWidget(createModelInfoSection());
    mainLayout->addWidget(createRenderSettingsSection());
    mainLayout->addWidget(createTransformControls());
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

void MainWindow::connectSignals()
{

}

void MainWindow::updatePropertiesPanel()
{

}

QGroupBox *MainWindow::createModelInfoSection()
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

QGroupBox *MainWindow::createRenderSettingsSection()
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
    connect(m_lightingCheck, &QCheckBox::toggled, this, &MainWindow::UpdateSceneLightingState);

    group->setLayout(layout);
    return group;
}

QGroupBox* MainWindow::createTransformControls()
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
                this, &MainWindow::updateTransformFromUI);
        return spin;
    };

    m_positionSpinboxX = initSpinBox(-1000, 1000, 0);
    m_positionSpinboxY = initSpinBox(-1000, 1000, 0);
    m_positionSpinboxZ = initSpinBox(-1000, 1000, 0);

    m_rotationSpinboxX = initSpinBox(-360, 360, 0);
    m_rotationSpinboxY = initSpinBox(-360, 360, 0);
    m_rotationSpinboxZ = initSpinBox(-360, 360, 0);

    m_scalingSpinboxX = initSpinBox(0.01, 10, 1);
    m_scalingSpinboxY = initSpinBox(0.01, 10, 1);
    m_scalingSpinboxZ = initSpinBox(0.01, 10, 1);

    layout->addRow("Position X:", m_positionSpinboxX);
    layout->addRow("Position Y:", m_positionSpinboxY);
    layout->addRow("Position Z:", m_positionSpinboxZ);

    layout->addRow("Rotation X:", m_rotationSpinboxX);
    layout->addRow("Rotation Y:", m_rotationSpinboxY);
    layout->addRow("Rotation Z:", m_rotationSpinboxZ);

    layout->addRow("Scale X:", m_scalingSpinboxX);
    layout->addRow("Scale Y:", m_scalingSpinboxY);
    layout->addRow("Scale Z:", m_scalingSpinboxZ);

    QPushButton *m_loadTextureButton = new QPushButton("Load Texture", this);
    layout->addRow(m_loadTextureButton);

    connect(m_loadTextureButton, &QPushButton::clicked, this, &MainWindow::loadTextureForSelectedModel);

    QPushButton* m_fitToViewButton = new QPushButton("Fit to View", this);
    layout->addRow(m_fitToViewButton);

    connect(m_fitToViewButton, &QPushButton::clicked, m_viewport, &ViewportWidget::fitToView);

    group->setLayout(layout);
    return group;
}

void MainWindow::openModelFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Файл не выбран."));
        return;
    }

    ModelController controller;
    if (!controller.loadModel(filePath)) {
        QString errorMsg = controller.getErrorString();
        QMessageBox::critical(this, "Ошибка загрузки", QString("Не удалось загрузить модель:\n%1").arg(errorMsg));
        return;
    }

    Model* model = new Model();
    *model = controller.getModel();

    m_models.append(model);
    m_modelTransforms.append(QMatrix4x4());

    updateModelList();
    m_viewport->setModels(m_models, m_modelTransforms);

    m_viewport->fitToView();
}

void MainWindow::updateModelList()
{
    m_explorerModel->removeRows(0, m_explorerModel->rowCount());
    for (int i = 0; i < m_models.size(); ++i) {
        QStandardItem* item = new QStandardItem(QString("Model %1").arg(i + 1));
        m_explorerModel->appendRow(item);
    }
}

void MainWindow::onExplorerModelSelected(const QModelIndex& index)
{
    m_currentModelIndex = index.row();

    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_models.size()) {
        const Model* model = m_models[m_currentModelIndex];

        m_modelNameLabel->setText(QString("Model %1").arg(m_currentModelIndex + 1));
        m_verticesLabel->setText(QString::number(model->m_vertices.size()));
        m_facesLabel->setText(QString::number(model->m_faceVertexIndices.size() / 3));

        m_textureCheck->setChecked(model->m_hasTexture);

        qDebug() << "[DEBUG] Выбрана модель #" << m_currentModelIndex;
        qDebug() << "hasTexture:" << model->m_hasTexture;
        qDebug() << "textureId:" << model->m_textureId;
        if (!model->isValid()) {
            QMessageBox::warning(this, "Предупреждение", "Модель повреждена или не содержит полигонов");
        }
    }
}

void MainWindow::UpdateSceneLightingState(bool checked)
{
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_models.size()) {
        m_models[m_currentModelIndex]->m_useNormals = checked;
        m_viewport->setModels(m_models, m_modelTransforms);
    }
}

void MainWindow::updateSelectedModelTextureState(bool checked)
{
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_models.size()) {
        m_models[m_currentModelIndex]->m_hasTexture = checked;
        m_viewport->setModels(m_models, m_modelTransforms);
    }
}

void MainWindow::updateTransformFromUI()
{
    if (m_currentModelIndex < 0 || m_currentModelIndex >= m_models.size())
        return;

    float px = m_positionSpinboxX->value();
    float py = m_positionSpinboxY->value();
    float pz = m_positionSpinboxZ->value();

    float rx = m_rotationSpinboxX->value();
    float ry = m_rotationSpinboxY->value();
    float rz = m_rotationSpinboxZ->value();

    float sx = m_scalingSpinboxX->value();
    float sy = m_scalingSpinboxY->value();
    float sz = m_scalingSpinboxZ->value();

    QMatrix4x4 transform;
    transform.translate(px, py, pz);
    transform.rotate(rx, 1, 0, 0);
    transform.rotate(ry, 0, 1, 0);
    transform.rotate(rz, 0, 0, 1);
    transform.scale(sx, sy, sz);

    m_modelTransforms[m_currentModelIndex] = transform;
    m_viewport->setModels(m_models, m_modelTransforms);
}

void MainWindow::loadTextureForSelectedModel()
{
    if (m_currentModelIndex < 0) {
        QMessageBox::warning(this, "Ошибка", "Не выбрана модель для загрузки текстуры.");
        return;
    }
    if (m_currentModelIndex >= m_models.size()) {
        QMessageBox::warning(this, "Ошибка", "Не выбрана модель для загрузки текстуры.");
        return;
    }
    QString texturePath = QFileDialog::getOpenFileName(this, tr("Open Texture"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (texturePath.isEmpty()) {
        QMessageBox::information(this, "Отмена", "Загрузка текстуры отменена.");
        return;
    }
    if (!m_viewport->loadTextureForModel(texturePath, m_currentModelIndex)) {
        QMessageBox::critical(this, "Ошибка", QString("Не удалось загрузить текстуру:\n%1").arg(texturePath));
        return;
    }

    m_models[m_currentModelIndex]->m_hasTexture = true;
    m_textureCheck->setChecked(true);
    m_viewport->setModels(m_models, m_modelTransforms);
}
}

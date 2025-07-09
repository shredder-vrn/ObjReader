#include "mainwindow.h"

namespace Viewer{

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("OBJ Viewer");
    resize(1400, 900);    

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_viewport = new ViewportWidget(this);
    m_viewport->fitToView();

    layout->addWidget(m_viewport);

    setCentralWidget(centralWidget);
    setupUserInterface();

    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* openAction = fileMenu->addAction("Open");

    connect(openAction, &QAction::triggered, this, &MainWindow::openModelFile);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::loadModel(const QString &filePath, ModelData &outData)
{
    QFile file(filePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Ошибка", QString("Не удалось открыть файл:\n%1").arg(filePath));
        return false;
    }

    QTextStream in(&file);
    QString errorLine;
    int errorLineNum = 0;

    if (!parseTokens(in, outData, &errorLine, &errorLineNum)) {
        LogError(QString("loadModel - ошибка парсинга на строке %1: %2").arg(errorLineNum).arg(errorLine));
        QMessageBox::critical(
            this,
            "Ошибка формата",
            QString("Ошибка в файле на строке %1:\n\"%2\"")
                .arg(errorLineNum).arg(errorLine)
        );
        return false;
    }

    if (outData.vertices().isEmpty()) {
        QMessageBox::warning(nullptr, "Предупреждение", "Модель не содержит вершин.");
        return false;
    }

    QVector<int> faceVertexIndices = outData.faceVertexIndices();
    QVector<int> faceTextureVertexIndices = outData.faceTextureVertexIndices();
    QVector<int> faceNormalIndices = outData.faceNormalIndices();
    QVector<int> polygonStarts = outData.polygonStarts();

    simpleTriangulateModel(
        outData.vertices(),
        faceVertexIndices,
        faceTextureVertexIndices,
        faceNormalIndices,
        polygonStarts
    );

    outData
        .setFaceVertexIndices(faceVertexIndices)
        .setFaceTextureVertexIndices(faceTextureVertexIndices)
        .setFaceNormalIndices(faceNormalIndices)
        .setPolygonStarts(polygonStarts);

    calculateNormals(outData);

    return true;
}

void MainWindow::openModelFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open OBJ File"), "", tr("Wavefront OBJ (*.obj)"));
    if (filePath.isEmpty()){
        QMessageBox::warning(this, "Файл не выбран", "Пользователь отменил выбор файла.");
        return;
    }

    ModelData modelData;
    if (!loadModel(filePath, modelData)){
        QMessageBox::critical(this, "Ошибка загрузки", "Не удалось загрузить модель из файла.");
        return;
    }

    ModelGL* modelGL = new ModelGL();
    modelGL->setModelData(new ModelData(modelData));

    m_modelsGL.append(modelGL);
    m_modelTransforms.append(QMatrix4x4());

    m_viewport->setModels(m_modelsGL, m_modelTransforms);
    updateModelList();
}

void MainWindow::updateModelList()
{
    m_explorerModel->removeRows(0, m_explorerModel->rowCount());
    for (int i = 0; i < m_modelsGL.size(); ++i) {
        QStandardItem* item = new QStandardItem(QString("Model %1").arg(i + 1));
        m_explorerModel->appendRow(item);
    }
}

void MainWindow::onExplorerModelSelected(const QModelIndex &index)
{
    m_currentModelIndex = index.row();
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_modelsGL.size()) {
        const ObjectGL *objectGL = m_modelsGL[m_currentModelIndex];
        const ModelGL *modelGL = dynamic_cast<const ModelGL*>(objectGL);
        if (!modelGL)
            return;
        m_modelNameLabel->setText(QString("Model %1").arg(m_currentModelIndex + 1));
        m_verticesLabel->setText(QString::number(modelGL->getModelData()->vertices().size()));
        m_facesLabel->setText(QString::number(modelGL->getModelData()->faceVertexIndices().size() / 3));
        m_textureCheck->setChecked(modelGL->hasTexture());

        m_wireframeCheck->setChecked(modelGL->wireframeMode());
    }
}

void MainWindow::UpdateSceneLightingState(bool checked)
{
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_modelsGL.size()) {
        m_modelsGL[m_currentModelIndex]->setUseNormals(checked);
        m_viewport->setModels(m_modelsGL, m_modelTransforms);
    }
}

void MainWindow::updateSelectedModelTextureState(bool checked)
{
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_modelsGL.size()) {
        m_modelsGL[m_currentModelIndex]->setHasTexture(checked);
        m_viewport->setModels(m_modelsGL, m_modelTransforms);
    }
}

void MainWindow::updateTransformFromUI()
{
    if (m_currentModelIndex < 0 || m_currentModelIndex >= m_modelsGL.size())
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
    m_viewport->setModels(m_modelsGL, m_modelTransforms);
}

void MainWindow::loadTestTextureForSelectedModel()
{
    if (m_currentModelIndex < 0 || m_currentModelIndex >= m_modelsGL.size()) {
        QMessageBox::warning(this, "Ошибка", "Модель не выбрана.");
        return;
    }

    const int size = 64;
    QImage image(size, size, QImage::Format_RGBA8888);

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            bool isBlack = ((x / 16) % 2 + (y / 16) % 2) % 2 == 0;
            image.setPixelColor(x, y, isBlack ? Qt::black : Qt::white);
        }
    }

    ModelGL* modelGL = dynamic_cast<ModelGL*>(m_modelsGL[m_currentModelIndex]);
    if (!modelGL) {
        QMessageBox::critical(this, "Ошибка", "Выбранный объект не поддерживает текстуры");
        return;
    }

    bool success = m_viewport->loadTextureForModel(m_currentModelIndex, image);
    if (!success) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать текстуру");
        return;
    }

    modelGL->setHasTexture(true);
    m_textureCheck->setChecked(true);
    m_viewport->setModels(m_modelsGL, m_modelTransforms);
}

void MainWindow::loadTextureForSelectedModel()
{
    if (m_currentModelIndex < 0 || m_currentModelIndex >= m_modelsGL.size())
        return;

    QString texturePath = QFileDialog::getOpenFileName(this, tr("Open Texture"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));

    if (texturePath.isEmpty()){
        QMessageBox::information(this, "Нет файла", "Пользователь не выбрал текстуру.");
        return;
    }

    if (!m_viewport->loadTextureForModel(m_currentModelIndex, texturePath)){
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить текстуру.");
        return;
    }

    m_modelsGL[m_currentModelIndex]->setHasTexture(true);
    m_textureCheck->setChecked(true);
    m_viewport->setModels(m_modelsGL, m_modelTransforms);
}

void MainWindow::toggleWireframe(bool enabled)
{
    if (m_currentModelIndex >= 0 && m_currentModelIndex < m_modelsGL.size()) {
        ModelGL* model = dynamic_cast<ModelGL*>(m_modelsGL[m_currentModelIndex]);
        if (model) {
            model->setWireframeMode(enabled);
        }
    }

    m_viewport->update();
}

void MainWindow::setupUserInterface()
{
    m_explorerDock = new QDockWidget("Explorer", this);
    m_explorerView = new QTreeView(m_explorerDock);
    m_explorerModel = new QStandardItemModel(this);

    m_explorerModel->setHorizontalHeaderLabels({"Models"});
    m_explorerView->setModel(m_explorerModel);
    m_explorerDock->setWidget(m_explorerView);

    addDockWidget(Qt::LeftDockWidgetArea, m_explorerDock);

    connect(m_explorerView->selectionModel(), &QItemSelectionModel::currentChanged, this, &MainWindow::onExplorerModelSelected);

    m_propertiesDock = new QDockWidget("Properties", this);
    QWidget *propertiesPanel = new QWidget(m_propertiesDock);
    QVBoxLayout *mainLayout = new QVBoxLayout(propertiesPanel);

    mainLayout->addWidget(createModelInfoSection());
    mainLayout->addWidget(createRenderSettingsSection());
    mainLayout->addWidget(createTransformControls());
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

void MainWindow::calculateNormals(ModelData &model)
{
    if (model.vertices().isEmpty() || model.faceVertexIndices().size() % 3 != 0) {
        return;
    }

    QVector<QVector3D> normals;
    normals.fill(QVector3D(0.0f, 0.0f, 0.0f), model.vertices().size());

    for (int i = 0; i < model.faceVertexIndices().size(); i += 3) {
        int idx0 = model.faceVertexIndices()[i];
        int idx1 = model.faceVertexIndices()[i + 1];
        int idx2 = model.faceVertexIndices()[i + 2];

        if (idx0 < 0 || idx1 < 0 || idx2 < 0 ||
            idx0 >= model.vertices().size() ||
            idx1 >= model.vertices().size() ||
            idx2 >= model.vertices().size()) {
            continue;
        }

        QVector3D v1 = model.vertices()[idx1] - model.vertices()[idx0];
        QVector3D v2 = model.vertices()[idx2] - model.vertices()[idx0];

        QVector3D normal = QVector3D::crossProduct(v2, v1).normalized();

        normals[idx0] += normal;
        normals[idx1] += normal;
        normals[idx2] += normal;
    }

    for (auto& n : normals) {
        n = n.normalized();
    }

    model.setNormals(normals);
}

QGroupBox *MainWindow::createModelInfoSection()
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

QGroupBox *MainWindow::createTransformControls()
{
    QGroupBox *group = new QGroupBox("Transform");
    QFormLayout *layout = new QFormLayout();

    auto initSpinBox = [this](double min, double max, double value) {
        QDoubleSpinBox *spin = new QDoubleSpinBox(this);
        spin->setRange(min, max);
        spin->setValue(value);
        spin->setSingleStep(0.1);
        spin->setDecimals(3);
        connect(spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::updateTransformFromUI);
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

    QPushButton *m_testTextureButton = new QPushButton("Load Test Texture", this);
    layout->addRow(m_testTextureButton);
    connect(m_testTextureButton, &QPushButton::clicked, this, &MainWindow::loadTestTextureForSelectedModel);


    QPushButton *m_fitToViewButton = new QPushButton("Fit to View", this);
    layout->addRow(m_fitToViewButton);

    connect(m_fitToViewButton, &QPushButton::clicked, m_viewport, &ViewportWidget::fitToView);

    group->setLayout(layout);
    return group;
}

QGroupBox *MainWindow::createRenderSettingsSection()
{
    QGroupBox *group = new QGroupBox("Render Options");
    QVBoxLayout *layout = new QVBoxLayout();

    m_wireframeCheck = new QCheckBox("Wireframe");
    m_textureCheck = new QCheckBox("Use Texture");
    m_lightingCheck = new QCheckBox("Lighting");
    m_normalsCheck = new QCheckBox("Show Normals");

    layout->addWidget(m_wireframeCheck);
    layout->addWidget(m_textureCheck);
    layout->addWidget(m_lightingCheck);
    //layout->addWidget(m_normalsCheck);

    connect(m_textureCheck, &QCheckBox::toggled, this, &MainWindow::updateSelectedModelTextureState);
    connect(m_lightingCheck, &QCheckBox::toggled, this, &MainWindow::UpdateSceneLightingState);
    connect(m_wireframeCheck, &QCheckBox::toggled, this, &MainWindow::toggleWireframe);

    group->setLayout(layout);

    return group;
}

}

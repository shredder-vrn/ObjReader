#ifndef MAINWINNDOW_H
#define MAINWINNDOW_H

#include <QMainWindow>
#include <QVector>
#include <QMatrix4x4>
#include <QDockWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QLabel>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>


#include "Viewport/viewport.h"
#include "ModelController/modelcontroller.h"

namespace Viewer{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openModelFile();
    void loadTextureForSelectedModel();
    void onExplorerModelSelected(const QModelIndex &index);
    void UpdateSceneLightingState(bool checked);
    void updateSelectedModelTextureState(bool checked);
    void updateTransformFromUI();
    void updateModelList();

private:
    void setupUserInterface();
    void connectSignals();
    void updatePropertiesPanel();

    QGroupBox *createModelInfoSection();
    QGroupBox *createRenderSettingsSection();
    QGroupBox *createTransformControls();

    int m_currentModelIndex = -1;

    ViewportWidget *m_viewport = nullptr;
    QVector<Model*> m_models;
    QVector<QMatrix4x4> m_modelTransforms;

    QDockWidget *m_propertiesDock = nullptr;
    QDockWidget *m_explorerDock = nullptr;
    QTreeView *m_explorerView = nullptr;
    QStandardItemModel *m_explorerModel = nullptr;

    QLabel *m_modelNameLabel = nullptr;
    QLabel *m_verticesLabel = nullptr;
    QLabel *m_facesLabel = nullptr;

    QCheckBox *m_wireframeCheck = nullptr;
    QCheckBox *m_textureCheck = nullptr;
    QCheckBox *m_lightingCheck = nullptr;
    QCheckBox *m_normalsCheck = nullptr;

    QDoubleSpinBox *m_positionSpinboxX = nullptr;
    QDoubleSpinBox *m_positionSpinboxY = nullptr;
    QDoubleSpinBox *m_positionSpinboxZ = nullptr;

    QDoubleSpinBox *m_rotationSpinboxX = nullptr;
    QDoubleSpinBox *m_rotationSpinboxY = nullptr;
    QDoubleSpinBox *m_rotationSpinboxZ = nullptr;

    QDoubleSpinBox *m_scalingSpinboxX = nullptr;
    QDoubleSpinBox *m_scalingSpinboxY = nullptr;
    QDoubleSpinBox *m_scalingSpinboxZ = nullptr;
};

}

#endif // MAINWINNDOW_H

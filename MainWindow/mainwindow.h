#ifndef MAINWINNDOW_H
#define MAINWINNDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTreeView>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QPushButton>


#include "Viewport/viewport.h"
#include "ModelController/modelcontroller.h"

namespace Viewer
{

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
    void toggleSceneLighting(bool checked);
    void updateTransformFromUI();
    void updateModelList();

private:
    void setupUserInterface();

    void connectSignals();
    void updatePropertiesPanel();

    QGroupBox* createModelInfoSection();
    QGroupBox* createRenderSettingsSection();
    QGroupBox* createTransformControls();

    int m_currentModelIndex = -1;

    ViewportWidget* m_viewport = nullptr;
    QVector<Model*> m_models;
    QVector<QMatrix4x4> m_modelTransforms;

    QDockWidget* m_propertiesDock = nullptr;
    QDockWidget* m_explorerDock = nullptr;
    QTreeView* m_explorerView = nullptr;
    QStandardItemModel* m_explorerModel = nullptr;

    QLabel* m_modelNameLabel = nullptr;
    QLabel* m_verticesLabel = nullptr;
    QLabel* m_facesLabel = nullptr;

    QCheckBox* m_wireframeCheck = nullptr;
    QCheckBox* m_textureCheck = nullptr;
    QCheckBox* m_lightingCheck = nullptr;
    QCheckBox* m_normalsCheck = nullptr;

    QDoubleSpinBox* m_posXSpin = nullptr;
    QDoubleSpinBox* m_posYSpin = nullptr;
    QDoubleSpinBox* m_posZSpin = nullptr;

    QDoubleSpinBox* m_rotXSpin = nullptr;
    QDoubleSpinBox* m_rotYSpin = nullptr;
    QDoubleSpinBox* m_rotZSpin = nullptr;

    QDoubleSpinBox* m_scaleXSpin = nullptr;
    QDoubleSpinBox* m_scaleYSpin = nullptr;
    QDoubleSpinBox* m_scaleZSpin = nullptr;

    void updateSelectedModelTextureState(bool checked);
};

}

#endif // MAINWINNDOW_H

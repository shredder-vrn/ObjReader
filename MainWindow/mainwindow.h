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
    ~MainWindow()
    {
        for (auto* model : m_models) {
            delete model;
        }
        m_models.clear();
    }

private slots:
    void openModel();

    void loadTextureForSelectedModel();

    void updateModelList();

    void onModelSelected(const QModelIndex &index);

    void onLightingToggled(bool checked);

    void updateModelTransform();


private:
    void setupDockWidgets();

    void onTextureCheckToggled(bool checked);

    void updateSelectedModelTextureState(bool checked);

    QGroupBox* createModelPropertiesSection();

    QGroupBox* createRenderOptionsSection();

    QGroupBox* createTransformSection();

    ViewportWidget* m_viewport = nullptr;

    QVector<Model*> m_models;
    QVector<QMatrix4x4> m_modelTransforms;
    int m_selectedModelIndex = -1;

    QDockWidget* m_propertiesDock = nullptr;
    QDockWidget* m_explorerDock = nullptr;

    QTreeView* m_explorerView = nullptr;

    QStandardItemModel* m_explorerModel = nullptr;

    QPushButton* m_loadTextureButton = nullptr;

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
};

}

#endif // MAINWINNDOW_H

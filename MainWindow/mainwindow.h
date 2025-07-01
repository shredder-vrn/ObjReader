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


#include "Viewport/viewport.h"

namespace Viewer
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void openModel();
    void updateRenderOptions();
    void updateModelTransform();

private:
    void setupDockWidgets();
    QGroupBox *createModelPropertiesSection();
    QGroupBox *createRenderOptionsSection();
    QGroupBox *createTransformSection();


    Viewport *m_viewport = nullptr;
    QDockWidget *m_explorerDock = nullptr;
    QTreeView *m_explorerView = nullptr;
    QDockWidget *m_propertiesDock = nullptr;
    QLabel *m_modelNameLabel = nullptr;
    QLabel *m_verticesLabel = nullptr;
    QLabel *m_facesLabel = nullptr;
    QCheckBox *m_wireframeCheck = nullptr;
    QCheckBox *m_textureCheck = nullptr;
    QCheckBox *m_lightingCheck = nullptr;
    QCheckBox *m_normalsCheck = nullptr;
    QDoubleSpinBox *m_posXSpin = nullptr;
    QDoubleSpinBox *m_posYSpin = nullptr;
    QDoubleSpinBox *m_posZSpin = nullptr;
    QDoubleSpinBox *m_rotXSpin = nullptr;
    QDoubleSpinBox *m_rotYSpin = nullptr;
    QDoubleSpinBox *m_rotZSpin = nullptr;
    QDoubleSpinBox *m_scaleXSpin = nullptr;
    QDoubleSpinBox *m_scaleYSpin = nullptr;
    QDoubleSpinBox *m_scaleZSpin = nullptr;
};
}

#endif // MAINWINNDOW_H

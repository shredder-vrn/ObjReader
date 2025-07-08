#ifndef VIEWER_MAINWINNDOW_H
#define VIEWER_MAINWINNDOW_H

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

#include "Viewport/logger.h"
#include "ObjReader/objreader.h"
#include "Viewport/viewport.h"
#include "Triangulate/triangulate.h"


namespace Viewer{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openModelFile();
    void loadTextureForSelectedModel();
    void onExplorerModelSelected(const QModelIndex &index);
    void UpdateSceneLightingState(bool checked);
    void updateSelectedModelTextureState(bool checked);
    void updateTransformFromUI();
    void updateModelList();
    void logFields() const;
    void translate(const QVector3D &translation);
    void rotate(float angle, const QVector3D &axis);
    void scale(const QVector3D &scalingFactors);
    void resetTransformations();
    void calculateNormals(ModelData &model);


    const ModelGL &getModelGL() const {return m_modelGL;};
    const QVector<ModelGL> &getModelGLs() const { return m_modelGLs; }

    bool loadModel(const QString &filePath);


private:
    void setupUserInterface();

    QGroupBox *createModelInfoSection();
    QGroupBox *createRenderSettingsSection();
    QGroupBox *createTransformControls();

    int m_currentModelIndex = -1;
    ViewportWidget *m_viewport = nullptr;
    QVector<QMatrix4x4> m_modelTransforms;
    QVector<ModelGL*> m_modelsGL;

    QString errorString;
    QVector<QMatrix4x4> m_modelMatrices;
    QMatrix4x4 m_modelControllerModelMatrix;
    QVector<ModelGL> m_modelGLs;
    ModelGL m_modelGL;
    ModelData m_modelData;


private:
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

#endif // VIEWER_MAINWINNDOW_H

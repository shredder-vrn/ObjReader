#include "mainwindowrefactoring.h"


MainWindowRefactoring::MainWindowRefactoring()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_tabWidget = new QTabWidget(this);
    m_addButton = new QPushButton("Добавить сцену", this);

    layout->addWidget(m_tabWidget);
    layout->addWidget(m_addButton);

    setCentralWidget(centralWidget);

    connect(m_addButton, &QPushButton::clicked, this, &MainWindowRefactoring::addScene);

    addScene();
}

void MainWindowRefactoring::addScene()
{
    SceneRefactoring* scene = new SceneRefactoring(this);
    ++m_sceneCount;

    QString tabName = QString("Сцена %1").arg(m_sceneCount);

    m_tabWidget->addTab(scene, tabName);
}

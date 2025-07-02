#ifndef MAINWINDOWREFACTORING_H
#define MAINWINDOWREFACTORING_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Model/model.h"
#include "Viewport/viewportrefactoring.h"


class MainWindowRefactoring : public QMainWindow
{
public:
    MainWindowRefactoring();

private:
    QTabWidget *m_tabWidget;
    QPushButton *m_addButton;
    int m_sceneCount = 0;
};

#endif // MAINWINDOWREFACTORING_H

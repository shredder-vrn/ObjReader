#ifndef MAINWINDOWREFACTORING_H
#define MAINWINDOWREFACTORING_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "Scene/scenerefactoring.h"

class MainWindowRefactoring : public QMainWindow
{
public:
    MainWindowRefactoring();
private slots:
    void addScene();

private:
    QTabWidget *m_tabWidget;
    QPushButton *m_addButton;
    int m_sceneCount = 0;
};

#endif // MAINWINDOWREFACTORING_H

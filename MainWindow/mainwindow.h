#ifndef MAINWINNDOW_H
#define MAINWINNDOW_H

#include <QMainWindow>

#include "Viewport/viewport.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void openModel();

private:
    Viewport *viewport = nullptr;
};

#endif // MAINWINNDOW_H

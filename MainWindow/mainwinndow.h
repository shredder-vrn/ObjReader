#ifndef MAINWINNDOW_H
#define MAINWINNDOW_H

#include <QMainWindow>

#include "Viewport/viewport.h"

namespace Viewer{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openModel();

private:
    Viewport *viewport = nullptr;
};
}

#endif // MAINWINNDOW_H

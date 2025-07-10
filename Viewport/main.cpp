#include <QApplication>

#include "MainWindow/mainwindow.h"

//! REIVIEW: тоже какой-то тестовый клсс. мб убрать?
class IntList
{
public:
    IntList()
    {
        m_data = new int[45];
    }
    ~IntList()
    {
        delete[] m_data;
    }

    IntList(const IntList &) = delete;
    IntList(IntList &&) = delete;
    IntList &operator=(const IntList &) = delete;
    IntList &operator=(IntList &&) = delete;

private:
    int *m_data = nullptr;
};

int main(int argc, char *argv[])
{
    {
        IntList list1;
        IntList list2;
    }

    QApplication app(argc, argv);
    Viewer::MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}

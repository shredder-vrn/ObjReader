#ifndef MODELTEST_H
#define MODELTEST_H

#include <QObject>

class ModelTest : public QObject
{
    Q_OBJECT

private slots:
    void testEmptyModel();
};


#endif // MODELTEST_H

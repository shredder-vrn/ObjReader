#include "modeltest.h"
#include "Model/model.h"

#include <QDebug>
#include <QtTest/QTest>

void ModelTest::testEmptyModel()
{
    Model model;
    Q_ASSERT(model.vertices.isEmpty());
}

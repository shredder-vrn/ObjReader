#ifndef TRIANGULATETEST_H
#define TRIANGULATETEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "Model/model.h"

class TriangulateTest : public QObject
{
    Q_OBJECT

private slots:
    void testTriangle_data();
    void testTriangle();

    void testQuad_data();
    void testQuad();

    void testPentagon_data();
    void testPentagon();

    void testConcavePolygon_data();
    void testConcavePolygon();

    void testDegeneratePolygon_data();
    void testDegeneratePolygon();

    void testEmptyInput();
    void testSingleVertex();
    void testTwoVertices();
};

#endif // TRIANGULATETEST_H

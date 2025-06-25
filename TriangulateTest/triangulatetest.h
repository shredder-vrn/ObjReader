#ifndef TRIANGULATETEST_H
#define TRIANGULATETEST_H

#include <QObject>
#include <QVector3D>
#include <QVector>

class TriangulateTest : public QObject
{
    Q_OBJECT
private slots:
    // Test cases
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
    void testColinearPoints();
    void testWindingOrder();
    void testEmptyInput();
    void testSingleVertex();
    void testTwoVertices();
    void testSelfIntersectingPolygon();

    // Helper functions
    static float calculatePolygonArea(const QVector<QVector3D>& vertices, const QVector<int>& indices);
    static bool verifyAllEdgesPresent(const QVector<QVector3D>& vertices,
                                    const QVector<int>& polygon,
                                    const QVector<int>& triangles);
};

#endif // TRIANGULATETEST_H

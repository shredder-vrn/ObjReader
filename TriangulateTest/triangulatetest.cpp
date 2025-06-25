#include "triangulatetest.h"
#include "Triangulate/triangulate.h"
#include <QtTest>
#include <algorithm>

QVector<int> createPolygon(const QVector<int>& indices) { return indices; }

float TriangulateTest::calculatePolygonArea(const QVector<QVector3D>& vertices, const QVector<int>& indices) {
    float area = 0.0f;
    int n = indices.size();
    for (int i = 0; i < n; ++i) {
        const QVector3D& current = vertices[indices[i]];
        const QVector3D& next = vertices[indices[(i + 1) % n]];
        area += current.x() * next.y() - next.x() * current.y();
    }
    return fabs(area) / 2.0f;
}

bool TriangulateTest::verifyAllEdgesPresent(const QVector<QVector3D>& vertices,
                                          const QVector<int>& polygon,
                                          const QVector<int>& triangles) {
    QVector<QPair<int, int>> originalEdges;
    QVector<QPair<int, int>> triangleEdges;

    // Collect original polygon edges
    int n = polygon.size();
    for (int i = 0; i < n; ++i) {
        int a = polygon[i];
        int b = polygon[(i + 1) % n];
        originalEdges.append(qMakePair(qMin(a, b), qMax(a, b)));
    }

    // Collect edges from triangles
    for (int i = 0; i < triangles.size(); i += 3) {
        for (int j = 0; j < 3; ++j) {
            int a = triangles[i + j];
            int b = triangles[i + (j + 1) % 3];
            triangleEdges.append(qMakePair(qMin(a, b), qMax(a, b)));
        }
    }

    // Check all original edges are present in triangulation
    for (const auto& edge : originalEdges) {
        if (!triangleEdges.contains(edge)) {
            return false;
        }
    }

    return true;
}

void TriangulateTest::testTriangle_data() {
    QTest::addColumn<QVector<QVector3D>>("vertices");
    QTest::addColumn<QVector<int>>("polygon");
    QTest::addColumn<QVector<int>>("expectedTriangles");

    QVector<QVector3D> triangle = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(0, 1, 0)
    };

    QTest::newRow("Simple triangle") << triangle << createPolygon({0, 1, 2}) << QVector<int>({0, 1, 2});
}

void TriangulateTest::testTriangle() {
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);
    QCOMPARE(result, expectedTriangles);
}

void TriangulateTest::testQuad_data() {
    QTest::addColumn<QVector<QVector3D>>("vertices");
    QTest::addColumn<QVector<int>>("polygon");
    QTest::addColumn<QVector<int>>("expectedTriangles");

    QVector<QVector3D> quad = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 1, 0),
        QVector3D(0, 1, 0)
    };

    QTest::newRow("Simple quad") << quad << createPolygon({0, 1, 2, 3}) << QVector<int>({0, 1, 2, 0, 2, 3});
}

void TriangulateTest::testQuad() {
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);
    QCOMPARE(result.size(), expectedTriangles.size());
    QVERIFY(verifyAllEdgesPresent(vertices, polygon, result));
}

void TriangulateTest::testPentagon_data() {
    QTest::addColumn<QVector<QVector3D>>("vertices");
    QTest::addColumn<QVector<int>>("polygon");
    QTest::addColumn<QVector<int>>("expectedTriangles");

    QVector<QVector3D> pentagon = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1.5, 0.5, 0),
        QVector3D(1, 1, 0),
        QVector3D(0, 1, 0)
    };

    QTest::newRow("Convex pentagon") << pentagon << createPolygon({0, 1, 2, 3, 4}) << QVector<int>({0, 1, 2, 0, 2, 3, 0, 3, 4});
}

void TriangulateTest::testPentagon() {
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);
    QCOMPARE(result.size(), expectedTriangles.size());
    QVERIFY(verifyAllEdgesPresent(vertices, polygon, result));
}

void TriangulateTest::testConcavePolygon_data() {
    QTest::addColumn<QVector<QVector3D>>("vertices");
    QTest::addColumn<QVector<int>>("polygon");
    QTest::addColumn<QVector<int>>("expectedTriangles");

    QVector<QVector3D> concave = {
        QVector3D(0, 0, 0),
        QVector3D(2, 0, 0),
        QVector3D(2, 2, 0),
        QVector3D(1, 1, 0),
        QVector3D(0, 2, 0)
    };

    QTest::newRow("Concave polygon") << concave << createPolygon({0, 1, 2, 3, 4}) << QVector<int>();
}

void TriangulateTest::testConcavePolygon() {
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);
    QVERIFY(!result.isEmpty());
    QCOMPARE(result.size(), (polygon.size() - 2) * 3);
    QVERIFY(verifyAllEdgesPresent(vertices, polygon, result));
}

void TriangulateTest::testDegeneratePolygon_data() {
    QTest::addColumn<QVector<QVector3D>>("vertices");
    QTest::addColumn<QVector<int>>("polygon");
    QTest::addColumn<QVector<int>>("expectedTriangles");

    QVector<QVector3D> line = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(2, 0, 0)
    };

    QTest::newRow("Degenerate colinear triangle") << line << createPolygon({0, 1, 2}) << QVector<int>();
}

void TriangulateTest::testDegeneratePolygon() {
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    float area = calculatePolygonArea(vertices, polygon);
    QVector<int> result = triangulatePolygon(vertices, polygon);

    if (qFuzzyIsNull(area)) {
        QVERIFY(result.isEmpty());
    } else {
        QVERIFY(!result.isEmpty());
    }
}

void TriangulateTest::testColinearPoints() {
    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(2, 0, 0),
        QVector3D(2, 1, 0)
    };

    QVector<int> polygon = {0, 1, 2, 3};
    QVector<int> result = triangulatePolygon(vertices, polygon);

    QCOMPARE(result.size(), 6); // 2 triangles
    QVERIFY(verifyAllEdgesPresent(vertices, polygon, result));
}

void TriangulateTest::testWindingOrder() {
    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(1, 0, 0),
        QVector3D(1, 1, 0),
        QVector3D(0, 1, 0)
    };

    QVector<int> polygonCCW = {0, 1, 2, 3};
    QVector<int> polygonCW = {0, 3, 2, 1};

    QVector<int> resultCCW = triangulatePolygon(vertices, polygonCCW);
    QVector<int> resultCW = triangulatePolygon(vertices, polygonCW);

    QCOMPARE(resultCCW.size(), 6);
    QCOMPARE(resultCW.size(), 6);
    QVERIFY(verifyAllEdgesPresent(vertices, polygonCCW, resultCCW));
    QVERIFY(verifyAllEdgesPresent(vertices, polygonCW, resultCW));
}

void TriangulateTest::testEmptyInput() {
    QVector<QVector3D> emptyVertices;
    QVector<int> emptyPolygon;
    QVector<int> result = triangulatePolygon(emptyVertices, emptyPolygon);
    QVERIFY(result.isEmpty());
}

void TriangulateTest::testSingleVertex() {
    QVector<QVector3D> singleVertex = {QVector3D(0, 0, 0)};
    QVector<int> polygon = {0};
    QVector<int> result = triangulatePolygon(singleVertex, polygon);
    QVERIFY(result.isEmpty());
}

void TriangulateTest::testTwoVertices() {
    QVector<QVector3D> twoVertices = {QVector3D(0, 0, 0), QVector3D(1, 0, 0)};
    QVector<int> polygon = {0, 1};
    QVector<int> result = triangulatePolygon(twoVertices, polygon);
    QVERIFY(result.isEmpty());
}

void TriangulateTest::testSelfIntersectingPolygon() {
    QVector<QVector3D> vertices = {
        QVector3D(0, 0, 0),
        QVector3D(2, 0, 0),
        QVector3D(2, 2, 0),
        QVector3D(0, 2, 0),
        QVector3D(1, 1, 0)
    };

    QVector<int> polygon = {0, 1, 2, 3, 4};
    QVector<int> result = triangulatePolygon(vertices, polygon);

    // Self-intersecting polygons may or may not be handled correctly
    // Just verify we got some result (implementation specific)
    QVERIFY(!result.isEmpty());
}



#include "triangulatetest.h"

#include <QTest>
#include "Triangulate/triangulate.h"  // Подключаем твой триангулятор

// Помощник для создания полигона из списка индексов
QVector<int> createPolygon(const QVector<int>& indices)
{
    return indices;
}

void TriangulateTest::testTriangle_data()
{
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

void TriangulateTest::testTriangle()
{
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);

    QCOMPARE(result.size(), expectedTriangles.size());
    for (int i = 0; i < result.size(); ++i) {
        QCOMPARE(result[i], expectedTriangles[i]);
    }
}

void TriangulateTest::testQuad_data()
{
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

void TriangulateTest::testQuad()
{
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);

    QCOMPARE(result.size(), expectedTriangles.size());
    for (int i = 0; i < result.size(); ++i) {
        QCOMPARE(result[i], expectedTriangles[i]);
    }
}

void TriangulateTest::testPentagon_data()
{
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

void TriangulateTest::testPentagon()
{
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);

    QVERIFY(!result.isEmpty());
    QVERIFY(result.size() == expectedTriangles.size());
    for (int i = 0; i < result.size(); ++i) {
        QCOMPARE(result[i], expectedTriangles[i]);
    }
}

void TriangulateTest::testConcavePolygon_data()
{
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

    QTest::newRow("Concave polygon") << concave << createPolygon({0, 1, 2, 3, 4}) << QVector<int>({0, 1, 2, 0, 2, 4, 4, 2, 3});
}

void TriangulateTest::testConcavePolygon()
{
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);

    QVERIFY(!result.isEmpty());
    QVERIFY(result.size() == expectedTriangles.size());
    for (int i = 0; i < result.size(); ++i) {
        QCOMPARE(result[i], expectedTriangles[i]);
    }
}

void TriangulateTest::testDegeneratePolygon_data()
{
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

void TriangulateTest::testDegeneratePolygon()
{
    QFETCH(QVector<QVector3D>, vertices);
    QFETCH(QVector<int>, polygon);
    QFETCH(QVector<int>, expectedTriangles);

    QVector<int> result = triangulatePolygon(vertices, polygon);

    QVERIFY(result.isEmpty());
}

void TriangulateTest::testEmptyInput()
{
    QVector<QVector3D> emptyVertices;
    QVector<int> emptyPolygon;

    QVector<int> result = triangulatePolygon(emptyVertices, emptyPolygon);

    QVERIFY(result.isEmpty());
}

void TriangulateTest::testSingleVertex()
{
    QVector<QVector3D> singleVertex = {QVector3D(0, 0, 0)};
    QVector<int> polygon = {0};

    QVector<int> result = triangulatePolygon(singleVertex, polygon);

    QVERIFY(result.isEmpty());
}

void TriangulateTest::testTwoVertices()
{
    QVector<QVector3D> twoVertices = {QVector3D(0, 0, 0), QVector3D(1, 0, 0)};
    QVector<int> polygon = {0, 1};

    QVector<int> result = triangulatePolygon(twoVertices, polygon);

    QVERIFY(result.isEmpty());
}

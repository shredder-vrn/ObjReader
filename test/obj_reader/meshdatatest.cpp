#include "meshdatatest.h"
#include "../src/obj_reader/meshdata.h"
#include "../src/obj_reader/objreader.h"

#include <QtTest/QtTest>
#include <QTest>

void MeshDataTest::emptyFaceTest()
{
    Model model;
    Face face;

    face.vertexIndices << 0 << 1;
    QVERIFY(face.vertexIndices.size() >= 3 == false);
}

void MeshDataTest::testValidFace()
{
    Model model;
    Face face;
    face.vertexIndices << 1 << 2 << 3;
    face.texCoordIndices << 1 << 2 << 3;
    face.normalIndices << 1 << 2 << 3;

    QVERIFY(face.vertexIndices.size() >= 3);
    QVERIFY(face.texCoordIndices.isEmpty() || face.texCoordIndices.size() == face.vertexIndices.size());
    QVERIFY(face.normalIndices.isEmpty() || face.normalIndices.size() == face.vertexIndices.size());

}

void MeshDataTest::testMismatchedTexCoordsCount()
{
    Model model;
    Face face;
    face.vertexIndices << 1 << 2 << 3;
    face.texCoordIndices << 2 << 3;

    QVERIFY(face.texCoordIndices.isEmpty() || face.texCoordIndices.size() == face.vertexIndices.size() == false);
}

void MeshDataTest::testMismatchedNormalCount()
{
    Model model;
    Face face;
    face.vertexIndices << 1 << 2 << 3;
    face.normalIndices << 2 << 3;

    QVERIFY(face.normalIndices.isEmpty() || face.normalIndices.size() == face.vertexIndices.size() == false);
}

void MeshDataTest::testIndexParsing_data()
{
    QTest::addColumn<QString>("token");
    QTest::addColumn<int>("expectedVertexIndex");
    QTest::addColumn<int>("expectedTexCoordIndex");
    QTest::addColumn<int>("expectedNormalIndex");

    QTest::newRow("v") << "1/2/3 1/3/4 1/4/5" << 0 << -1 << -1;
}

void MeshDataTest::testIndexParsing()
{
    QFETCH(QString, token);
    QFETCH(int, expectedVertexIndex);
    QFETCH(int, expectedTexCoordIndex);
    QFETCH(int, expectedNormalIndex);

    QStringList tokens = QStringList{"f", token};

    Model model;
    QString errorMessage;
    int lineNum = 1;

    //bool result = ObjReader::parseFaceLine(tokens, model, "", lineNum, errorMessage);

    //QVERIFY(result);
}

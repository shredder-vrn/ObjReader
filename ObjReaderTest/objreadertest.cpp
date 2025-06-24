#include "objreadertest.h"
#include "Model/model.h"
#include "ObjReader/objreader.h"

#include <QTest>

void ObjReaderTest::testParseVertexLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid vertex") << QStringList{"v", "1.0", "2.0", "3.0"} << true;
    QTest::newRow("Not enough coords") << QStringList{"v", "1.0", "2.0"} << false;
    QTest::newRow("Too many coords") << QStringList{"v", "1.0", "2.0", "3.0", "4.0"} << false;
    QTest::newRow("Invalid number") << QStringList{"v", "1.0", "abc", "3.0"} << false;
    QTest::newRow("Empty value") << QStringList{"v", "1.0", "", "3.0"} << false;
    QTest::newRow("Large number") << QStringList{"v", "1.0", "999999999999999999999999", "3.0"} << true;
    QTest::newRow("Negative number") << QStringList{"v", "-1.0", "-2.0", "-3.0"} << true;
}

void ObjReaderTest::testParseVertexLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    ModelV2 model;
    int lineNum = 1;

    bool result = parseVertex(tokens, model.vertexData, lineNum, QString());
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseTexCoordLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid texcoord") << QStringList{"vt", "0.5", "0.5"} << true;
    QTest::newRow("Invalid v") << QStringList{"vt", "0.5", "abc"} << false;
    QTest::newRow("Missing coord") << QStringList{"vt", "0.5"} << false;
    QTest::newRow("Empty coord") << QStringList{"vt", "", "0.5"} << false;
    QTest::newRow("u > 1") << QStringList{"vt", "1.1", "0.5"} << true;
    QTest::newRow("u < 0") << QStringList{"vt", "-0.1", "0.5"} << true;
}

void ObjReaderTest::testParseTexCoordLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    ModelV2 model;
    int lineNum = 1;
    bool result = parseTexCoord(tokens, model.texCoordData, lineNum, QString());
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseNormalLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid normal") << QStringList{"vn", "0.0", "1.0", "0.0"} << true;
    QTest::newRow("Invalid y") << QStringList{"vn", "0.0", "abc", "0.0"} << false;
    QTest::newRow("Missing coord") << QStringList{"vn", "0.0", "1.0"} << false;
    QTest::newRow("Empty coord") << QStringList{"vn", "0.0", "", "0.0"} << false;
    QTest::newRow("Long vector") << QStringList{"vn", "2.0", "2.0", "2.0"} << true;
    QTest::newRow("Too many coords") << QStringList{"vn", "0.0", "1.0", "0.0", "0.0"} << false;
}

void ObjReaderTest::testParseNormalLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    ModelV2 model;
    int lineNum = 1;

    bool result = parseNormal(tokens, model.normalData, lineNum, QString());
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseFaceLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("f v v v") << QStringList{"f", "1", "2", "3"} << true;
    QTest::newRow("f v/vt/vn") << QStringList{"f", "1/1/1", "2/2/2", "3/3/3"} << true;
    QTest::newRow("f v//vn") << QStringList{"f", "1//1", "2//2", "3//3"} << true;
    QTest::newRow("f v/vt") << QStringList{"f", "1/1", "2/2", "3/3"} << true;
    QTest::newRow("Invalid index") << QStringList{"f", "abc", "2", "3"} << false;
    QTest::newRow("Zero index") << QStringList{"f", "0", "2", "3"} << false;
    QTest::newRow("Negative index") << QStringList{"f", "-1", "2", "3"} << false;
    QTest::newRow("Duplicate vertices") << QStringList{"f", "1", "1", "1"} << true;
    QTest::newRow("Only two vertices") << QStringList{"f", "1", "2"} << false;
}

void ObjReaderTest::testParseFaceLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    ModelV2 model;
    int lineNum = 1;

    bool result = parseFace(
                tokens,
                model.faceVertexIndices,
                model.faceTexCoordIndices,
                model.faceNormalIndices,
                model.polygonStarts,
                model.polygonLengths,
                lineNum,
                QString());
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testValidateModel_data()
{
    QTest::addColumn<ModelV2>("model");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Empty model") << ModelV2() << false;

    {
        ModelV2 model;
        model.vertexData << 0.0f << 0.0f << 0.0f;
        QTest::newRow("Only vertices") << model << false;
    }

    {
        ModelV2 model;
        model.vertexData << 0.0f << 0.0f << 0.0f
                         << 1.0f << 0.0f << 0.0f
                         << 0.0f << 1.0f << 0.0f;

        model.faceVertexIndices << 0 << 1 << 2;
        model.polygonStarts << 0;
        model.polygonLengths << 3;

        model.texCoordData << 0.0f << 0.0f
                          << 1.0f << 0.0f
                          << 0.0f << 1.0f;
        model.faceTexCoordIndices << 0 << 1 << 2;

        model.normalData << 0.0f << 0.0f << 1.0f;
        model.faceNormalIndices << 0 << 0 << 0;

        QTest::newRow("Valid triangle with normals and texcoords") << model << true;
    }

    {
        ModelV2 model;
        model.vertexData << 0.0f << 0.0f << 0.0f;
        model.faceVertexIndices << 1;
        model.polygonStarts << 0;
        model.polygonLengths << 1;

        QTest::newRow("Index out of bounds") << model << false;
    }
}

void ObjReaderTest::testValidateModel()
{
    QFETCH(ModelV2, model);
    QFETCH(bool, expectedResult);

    bool result = validateModel(model);
    QCOMPARE(result, expectedResult);
}

#include "objreadertest.h"
#include "../src/obj_reader/meshdata.h"
#include "../src/obj_reader/objreader.h"

#include <QTest>


void ObjReaderTest::testParseVertexLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expertedResult");

    QTest::newRow("valid vertex") << QStringList{"v", "1.0", "2.0", "3.0"} << true;
    QTest::newRow("inbalid y coord") << QStringList{"v", "asd", "2.0", "3.0"} << false;
    QTest::newRow("not number z") << QStringList{"v", "1.0", "2.0", ""} << false;
    QTest::newRow("too few tokens") << QStringList{"v", "1.0", "2.0"} << true;
}

void ObjReaderTest::testParseVertexLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expertedResult);

    Model model;
   // bool result = parseVertexLine(tokens, "", 1, model);


}

void ObjReaderTest::testParseFaceLine1()
{

}

void ObjReaderTest::testParseFaceLine2()
{

}

void ObjReaderTest::testParseFaceLine3()
{

}

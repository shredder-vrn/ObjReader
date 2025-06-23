#include "objreadertest.h"
#include "../src/obj_reader/meshdata.h"
#include "../src/obj_reader/objreader.h"

#include <QTest>

void ObjReaderTest::testParseVertexLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Корректная вершина") << QStringList{"v", "1.0", "2.0", "3.0"} << true;
    QTest::newRow("Недостаточно координат") << QStringList{"v", "1.0", "2.0"} << false;
    QTest::newRow("Слишком много координат") << QStringList{"vn", "0.0", "1.0", "0.0", "0.0"} << false;
    QTest::newRow("Нечисловое значение") << QStringList{"v", "1.0", "abc", "3.0"} << false;
    QTest::newRow("Пустая координата") << QStringList{"v", "1.0", "", "3.0"} << false;
    QTest::newRow("Очень большое число") << QStringList{"v", "1.0", "2.0", "999999999999999999999999"} << true;
    QTest::newRow("Очень маленькое число") << QStringList{"v", "1.0", "-999999999999999999999999", "3.0"} << true;
}

void ObjReaderTest::testParseVertexLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    Model model;
    int lineNum;

    bool result = ObjReader::parseVertexLine(tokens, model, QString(), lineNum);
    QCOMPARE(result, expectedResult);
}


void ObjReaderTest::testParseTexCoordLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Корректная текстурная координата") << QStringList{"vt", "0.5", "0.5"} << true;
    QTest::newRow("Некорректное значение v") << QStringList{"vt", "0.5", "abc"} << false;
    QTest::newRow("Мало координат") << QStringList{"vt", "0.5"} << false;
    QTest::newRow("Пустая координата") << QStringList{"vt", "", "0.5"} << false;
    QTest::newRow("u больше 1") << QStringList{"vt", "1.1", "0.5"} << true;
    QTest::newRow("u меньше 0") << QStringList{"vt", "-0.1", "0.5"} << true;
}

void ObjReaderTest::testParseTexCoordLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    Model model;
    int lineNum;

    bool result = ObjReader::parseTexCoordLine(tokens, model, QString(), lineNum);
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseNormalLine_data()
{
    QTest::addColumn<QStringList>("tokens");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Корректная нормаль") << QStringList{"vn", "0.0", "1.0", "0.0"} << true;
    QTest::newRow("Некорректное значение y") << QStringList{"vn", "0.0", "abc", "0.0"} << false;
    QTest::newRow("Мало координат") << QStringList{"vn", "0.0", "1.0"} << false;
    QTest::newRow("Пустое значение") << QStringList{"vn", "0.0", "", "0.0"} << false;
    QTest::newRow("Длинный вектор (не нормализованный)") << QStringList{"vn", "2.0", "2.0", "2.0"} << true;
    QTest::newRow("Слишком много координат") << QStringList{"vn", "0.0", "1.0", "0.0", "0.0"} << false;
}

void ObjReaderTest::testParseNormalLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    Model model;
    int lineNum;

    bool result = ObjReader::parseNormalLine(tokens, model, QString(), lineNum);
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
    QTest::newRow("Невалидный индекс") << QStringList{"f", "abc", "2", "3"} << false;
    QTest::newRow("Индекс равен нулю") << QStringList{"f", "0", "2", "3"} << false;
    QTest::newRow("Отрицательный индекс") << QStringList{"f", "-1", "2", "3"} << false;
    QTest::newRow("Повторяющиеся вершины") << QStringList{"f", "1", "1", "1"} << true;
    QTest::newRow("Грань с 2 вершинами") << QStringList{"f", "1", "2"} << false;
}

void ObjReaderTest::testParseFaceLine()
{
    QFETCH(QStringList, tokens);
    QFETCH(bool, expectedResult);

    Model model;
    QString errorMessage;
    int lineNum = 1;
    bool result = ObjReader::parseFaceLine(tokens, model, QString(), lineNum, errorMessage);
    QCOMPARE(result, expectedResult);
}




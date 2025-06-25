#include "objreadertest.h"

#include <QRegularExpression>
#include <QTest>
#include <QTextStream>

#include "Model/model.h"
#include "ObjReader/objreader.h"


static bool parseSingleLine(const QString& line, Model &model)
{
    QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (tokens.isEmpty())
        return false;

    QString mutableLine = line;
    QTextStream in(&mutableLine);

    int lineNum = 1;

    const QString type = tokens[0];

    if (type == "v")
        return parseVertex(tokens, line, model.vertices, lineNum);
    if (type == "vt")
        return parseTexCoord(tokens, line, model.textureVertices, lineNum);
    if (type == "vn")
        return parseNormal(tokens, line, model.normals, lineNum);
    if (type == "f")
        return parseFace(tokens, line,
                         model.faceVertexIndices,
                         model.faceTexCoordIndices,
                         model.faceNormalIndices,
                         model.polygonStarts,
                         model.polygonLengths,
                         lineNum);


    return false;
}

void ObjReaderTest::testParseVertexLine_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid vertex") << "v 1.0 2.0 3.0" << true;
    QTest::newRow("Valid with negative values") << "v -1.5 -2.5 -3.5" << true;
    QTest::newRow("Valid with zero") << "v 0.0 0.0 0.0" << true;
    QTest::newRow("Valid scientific notation") << "v 1e3 2E-2 3.0" << true;
    QTest::newRow("Valid large numbers") << "v 123456789.0 987654321.0 1.0" << true;
    QTest::newRow("Scientific notation with sign") << "v -1e+3 2.0 3.0" << true;
    QTest::newRow("Negative zero") << "v -0.0 -0.0 -0.0" << true;
    QTest::newRow("Only exponents") << "v 1e5 2e10 3e-3" << true;

    QTest::newRow("Too many coords") << "v 1.0 2.0 3.0 4.0" << false;
    QTest::newRow("Not enough coords") << "v 1.0 2.0" << false;
    QTest::newRow("Zero coords") << "v" << false;
    QTest::newRow("Only one coord") << "v 1.0" << false;
    QTest::newRow("Whitespace only after v") << "v  " << false;

    QTest::newRow("Two extra spaces between tokens") << "v  1.0   2.0    3.0" << true;
    QTest::newRow("Multiple tabs and spaces") << "v\t1.0\t\t2.0 \t 3.0" << true;
    QTest::newRow("Leading spaces") << "     v 1.0 2.0 3.0" << true;
    QTest::newRow("Extra trailing space") << "v 1.0 2.0 3.0 " << true;
    QTest::newRow("Empty between coords") << "v 1.0  3.0   3.0" << true;
    QTest::newRow("Line with many spaces") << "v           1.0          2.0         3.0       " << true;
    QTest::newRow("Line with mixed whitespace") << "v\t1.0 \t 2.0 \v\v 3.0" << true;
    QTest::newRow("Carriage return") << "v 1.0\r2.0 3.0" << true;
    QTest::newRow("Newline in line") << "v 1.0\n2.0 3.0" << true;

    QTest::newRow("Trailing comment") << "v 1.0 2.0 3.0 #comment here" << true;
    QTest::newRow("Line with inline comment after valid") << "v 1.0 2.0 3.0 # some comment" << true;
    QTest::newRow("Comment in the middle") << "v 1.0 #x 2.0 3.0" << false;
    QTest::newRow("Line with comment and garbage") << "v 1.0 2.0 3.0 #garbage@#$%^&*" << true;

    QTest::newRow("Invalid x coord") << "v abc 2.0 3.0" << false;
    QTest::newRow("Invalid y coord") << "v 1.0 abc 3.0" << false;
    QTest::newRow("Invalid z coord") << "v 1.0 2.0 xyz" << false;
    QTest::newRow("Mixed invalid") << "v 1.0 abc xyz" << false;
    QTest::newRow("Multiple dots in number") << "v 1.0.0 2.0 3.0" << false;
    QTest::newRow("Hexadecimal number") << "v 0x1A 2.0 3.0" << false;
    QTest::newRow("Binary number") << "v 0b1010 2.0 3.0" << false;
    QTest::newRow("Infinity") << "v inf 2.0 3.0" << false;
    QTest::newRow("NaN") << "v NaN 2.0 3.0" << false;
    QTest::newRow("Exponent without base") << "v e5 2.0 3.0" << false;
    QTest::newRow("Exponent without digits") << "v 1e 2.0 3.0" << false;
    QTest::newRow("Letter after number") << "v 1a 2b 3c" << false;
    QTest::newRow("Number with %") << "v 100% 2.0 3.0" << false;
    QTest::newRow("Currency symbol") << "v $1.0 2.0 3.0" << false;
    QTest::newRow("Units specified") << "v 1.0m 2.0cm 3.0km" << false;

    QTest::newRow("Number with plus sign") << "v +1.0 +2.0 +3.0" << true;
    QTest::newRow("Number with leading dot") << "v .5 2.0 3.0" << true;
    QTest::newRow("Number with trailing dot") << "v 1. 2.0 3.0" << true;
    QTest::newRow("Very small float") << "v 0.0000000000001 0.0 0.0" << true;
    QTest::newRow("Scientific notation negative exponent") << "v 1e-10 2.0 3.0" << true;
    QTest::newRow("Scientific notation positive exponent") << "v 1e+10 2.0 3.0" << true;

    QTest::newRow("Missing space after v") << "v1.0 2.0 3.0" << false;
    QTest::newRow("Multiple v's") << "vv 1.0 2.0 3.0" << false;
    QTest::newRow("Uppercase V") << "V 1.0 2.0 3.0" << false;
    QTest::newRow("With slash like face") << "v 1/2 2/3 3/4" << false;
    QTest::newRow("With slash and decimal") << "v 1.0/2.0 2.0/3.0 3.0/4.0" << false;
    QTest::newRow("With colon") << "v 1:2 2:3 3:4" << false;
    QTest::newRow("With underscores") << "v 1_0 2_0 3_0" << false;
    QTest::newRow("With commas") << "v 1,0 2,0 3,0" << false;
    QTest::newRow("Numbers with thousand separators") << "v 1,000.0 2,000.0 3,000.0" << false;
    QTest::newRow("Unicode chars") << "v 1.0 2.0 α" << false;
    QTest::newRow("Escaped char") << "v 1.0\\n 2.0 3.0" << false;
    QTest::newRow("Tab inside token") << "v 1.\t0 2.0 3.0" << false;
    QTest::newRow("Backspace in number") << "v 1\b.0 2.0 3.0" << false;
    QTest::newRow("Hidden null char") << "v 1.0\0 2.0 3.0" << false;
    QTest::newRow("String with quotes") << "v \"1.0\" 2.0 3.0" << false;
    QTest::newRow("String with braces") << "v (1.0) 2.0 {3.0}" << false;

    QTest::newRow("Trailing garbage") << "v 1.0 2.0 3.0 asd" << false;
    QTest::newRow("Trailing numbers") << "v 1.0 2.0 3.0 5" << false;
    QTest::newRow("Very long line") << "v 1.0 2.0 3.0 asdasdasdasdasdasdasdasdasdasdasdasdasdasdasd" << false;
}

void ObjReaderTest::testParseVertexLine()
{
    QFETCH(QString, line);
    QFETCH(bool, expectedResult);

    Model model;
    bool result = parseSingleLine(line, model);

    QCOMPARE(result, expectedResult);

    if (expectedResult) {
        QVERIFY(!model.vertices.isEmpty());
        qDebug() << "Parsed vertex:" << model.vertices.last();
    }
}

void ObjReaderTest::testParseTexCoordLine_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid texcoord") << "vt 0.5 0.5" << true;
    QTest::newRow("Valid with zero") << "vt 0.0 0.0" << true;
    QTest::newRow("Valid with one") << "vt 1.0 1.0" << true;
    QTest::newRow("Valid negative") << "vt -0.5 -0.5" << true;
    QTest::newRow("Valid extra precision") << "vt 0.333333 0.666666" << true;
    QTest::newRow("Valid scientific notation") << "vt 1e2 2E-1" << true;
    QTest::newRow("Negative zero") << "vt -0.0 -0.0" << true;
    QTest::newRow("Only exponents") << "vt 1e5 2e10" << true;
    QTest::newRow("Scientific notation with sign") << "vt -1e+3 0.5" << true;

    QTest::newRow("Too many coords") << "vt 0.5 0.5 0.5" << false;
    QTest::newRow("Not enough coords") << "vt 0.5" << false;
    QTest::newRow("Zero coords") << "vt" << false;
    QTest::newRow("Whitespace only after vt") << "vt  " << false;

    QTest::newRow("Two extra spaces between tokens") << "vt  0.5   0.5" << true;
    QTest::newRow("Multiple tabs and spaces") << "vt\t0.5\t\t0.5" << true;
    QTest::newRow("Leading spaces") << "     vt 0.5 0.5" << true;
    QTest::newRow("Extra trailing space") << "vt 0.5 0.5 " << true;
    QTest::newRow("Empty between coords") << "vt 0.5  0.5" << true;
    QTest::newRow("Carriage return") << "vt 0.5\r0.5" << true;
    QTest::newRow("Newline in line") << "vt 0.5\n0.5" << true;

    QTest::newRow("Trailing comment") << "vt 0.5 0.5 #comment here" << true;
    QTest::newRow("Line with inline comment after valid") << "vt 0.5 0.5 # some comment" << true;
    QTest::newRow("Comment in the middle") << "vt 0.5 #x 0.5" << false;
    QTest::newRow("Line with comments at start") << "#vt 0.5 0.5" << false;
    QTest::newRow("Line with comment and garbage") << "vt 0.5 0.5 #garbage@#$%^&*" << true;

    QTest::newRow("Invalid u coord") << "vt abc 0.5" << false;
    QTest::newRow("Invalid v coord") << "vt 0.5 xyz" << false;
    QTest::newRow("Mixed invalid") << "vt abc xyz" << false;
    QTest::newRow("Infinity") << "vt inf 0.5" << false;
    QTest::newRow("NaN") << "vt NaN 0.5" << false;
    QTest::newRow("Hexadecimal number") << "vt 0xA 0xB" << false;
    QTest::newRow("Binary number") << "vt 0b1010 0.5" << false;
    QTest::newRow("Hex number") << "vt 0x1A 0.5" << false;
    QTest::newRow("Letter after number") << "vt 0a 0b" << false;
    QTest::newRow("With letters in numbers") << "vt 0a.5 0b.5" << false;
    QTest::newRow("Multiple decimal points") << "vt 0.5.0 0.5" << false;
    QTest::newRow("Exponent without base") << "vt e5 0.5" << false;
    QTest::newRow("Exponent without digits") << "vt 1e 0.5" << false;
    QTest::newRow("Multiple dots in number") << "vt 0.5.0 0.5" << false;
    QTest::newRow("String with quotes") << "vt \"0.5\" 0.5" << false;
    QTest::newRow("String with braces") << "vt (0.5) {0.5}" << false;
    QTest::newRow("Unicode chars") << "vt α β" << false;
    QTest::newRow("Escaped char") << "vt 0.5\\n 0.5" << false;
    QTest::newRow("Tab inside token") << "vt 0.\t5 0.5" << false;
    QTest::newRow("Backspace in number") << "vt 0\b.5 0.5" << false;
    QTest::newRow("Hidden null char") << "vt 0.5\0 0.5" << false;
    QTest::newRow("Units specified") << "vt 0.5m 0.5cm" << false;
    QTest::newRow("Currency symbol") << "vt $0.5 0.5" << false;
    QTest::newRow("Number with %") << "vt 50% 50%" << false;
    QTest::newRow("Spaces in numbers") << "vt 0 . 5 0.5" << false;

    QTest::newRow("Number with plus sign") << "vt +0.5 +0.5" << true;
    QTest::newRow("Number with leading dot") << "vt .5 0.5" << true;
    QTest::newRow("Number with trailing dot") << "vt 0. 0.5" << true;

    QTest::newRow("Multiple vt's") << "vvt 0.5 0.5" << false;
    QTest::newRow("Uppercase VT") << "VT 0.5 0.5" << false; // OBJ: только lowercase 'vt'
    QTest::newRow("With slash like face") << "vt 0.5/0.5 0.5/0.5" << false;
    QTest::newRow("With colon") << "vt 0.5:0.5 0.5:0.5" << false;
    QTest::newRow("With underscores") << "vt 0_5 0_5" << false;

    QTest::newRow("Very small float") << "vt 0.0000000000001 0.0" << true;
    QTest::newRow("Scientific notation negative exponent") << "vt 1e-10 0.5" << true;
    QTest::newRow("Scientific notation positive exponent") << "vt 1e+10 0.5" << true;
    QTest::newRow("Trailing garbage") << "vt 0.5 0.5 asd" << false;
    QTest::newRow("Trailing numbers") << "vt 0.5 0.5 5" << false;
    QTest::newRow("Missing space after vt") << "vt0.5 0.5" << false;
}

void ObjReaderTest::testParseTexCoordLine()
{
    QFETCH(QString, line);
    QFETCH(bool, expectedResult);

    Model model;
    bool result = parseSingleLine(line, model);

    QCOMPARE(result, expectedResult);

    if (expectedResult) {
        QVERIFY(!model.textureVertices.isEmpty());
        qDebug() << "Parsed textureVertex:" << model.textureVertices.last();
    }
}

void ObjReaderTest::testParseNormalLine_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Valid normal") << "vn 0.0 1.0 0.0" << true;
    QTest::newRow("Valid with negative values") << "vn -0.5 -1.0 -0.5" << true;
    QTest::newRow("Valid scientific notation") << "vn 1e0 2E-1 3.0" << true;
    QTest::newRow("Valid large numbers") << "vn 999999.0 123456.789 0.000001" << true;
    QTest::newRow("Negative zero") << "vn -0.0 -0.0 -0.0" << true;
    QTest::newRow("Scientific notation with sign") << "vn -1e+3 0.0 0.0" << true;

    QTest::newRow("Too many coords") << "vn 0.0 1.0 0.0 0.0" << false;
    QTest::newRow("Missing coord") << "vn 0.0 1.0" << false;
    QTest::newRow("Zero coords") << "vn" << false;
    QTest::newRow("Only one coord") << "vn 0.0" << false;

    QTest::newRow("Two extra spaces between tokens") << "vn  0.0   1.0    0.0" << true;
    QTest::newRow("Multiple tabs and spaces") << "vn\t0.0\t\t1.0 \t 0.0" << true;
    QTest::newRow("Leading spaces") << "     vn 0.0 1.0 0.0" << true;
    QTest::newRow("Tab inside token") << "vn 0.0\t0.0 0.0" << true;
    QTest::newRow("Carriage return") << "vn 0.0\r0.0 0.0" << true;
    QTest::newRow("Newline in line") << "vn 0.0\n0.0 0.0" << true;
    QTest::newRow("Whitespace only after vn") << "vn  " << false;
    QTest::newRow("Missing space after vn") << "vn0.0 1.0 0.0" << false;
    QTest::newRow("Hidden null char") << "vn 0.0\0 0.0 0.0" << false;

    QTest::newRow("Line with inline comment after valid") << "vn 0.0 0.0 0.0 # some comment" << true;
    QTest::newRow("Line with comment and garbage") << "vn 0.0 0.0 0.0 #garbage@#$%^&*" << true;
    QTest::newRow("Line with comments at start") << "#vn 0.0 0.0 0.0" << false;
    QTest::newRow("Comment in the middle") << "vn 0.0 #x 1.0 0.0" << false;

    QTest::newRow("Invalid y") << "vn 0.0 abc 0.0" << false;
    QTest::newRow("Mixed invalid") << "vn 0.0 abc xyz" << false;
    QTest::newRow("Unicode chars") << "vn 0.0 α 0.0" << false;
    QTest::newRow("String with quotes") << "vn \"0.0\" 0.0 0.0" << false;
    QTest::newRow("String with braces") << "vn (0.0) {0.0} [0.0]" << false;
    QTest::newRow("Letter after number") << "vn 1a 2b 3c" << false;
    QTest::newRow("Units specified") << "vn 0.0m 0.0cm 0.0km" << false;
    QTest::newRow("With underscores") << "vn 1_0 2_0 3_0" << false;

    QTest::newRow("Multiple decimal points") << "vn 0.0.0 0.0 0.0" << false;
    QTest::newRow("Hexadecimal number") << "vn 0x1A 2.0 3.0" << false;
    QTest::newRow("Binary number") << "vn 0b1010 0.0 0.0" << false;
    QTest::newRow("Exponent without base") << "vn e5 0.0 0.0" << false;
    QTest::newRow("Exponent without digits") << "vn 1e 0.0 0.0" << false;
    QTest::newRow("Infinity") << "vn inf 0.0 0.0" << false;
    QTest::newRow("Negative infinity") << "vn -inf 0.0 0.0" << false;
    QTest::newRow("NaN") << "vn NaN 0.0 0.0" << false;
    QTest::newRow("Currency symbol") << "vn $0.0 0.0 0.0" << false;
    QTest::newRow("Number with %") << "vn 100% 0.0 0.0" << false;
    QTest::newRow("Spaces in numbers") << "vn 0 . 0 0.0 0.0" << false;

    QTest::newRow("Uppercase VN") << "VN 0.0 1.0 0.0" << false;
    QTest::newRow("With slash like face") << "vn 1/2 2/3 3/4" << false;
    QTest::newRow("With commas") << "vn 1,0 2,0 3,0" << false;
    QTest::newRow("Escaped char") << "vn 0.0\\n 0.0 0.0" << false;
    QTest::newRow("Backspace in number") << "vn 0\b.0 0.0 0.0" << false;

    QTest::newRow("Empty value") << "vn 0.0  0.0" << false;
    QTest::newRow("Trailing garbage") << "vn 0.0 1.0 0.0 asd" << false;
    QTest::newRow("Very small float") << "vn 0.0000000000001 0.0 0.0" << true;
    QTest::newRow("Scientific notation negative exponent") << "vn 1e-10 0.0 0.0" << true;
    QTest::newRow("Scientific notation positive exponent") << "vn 1e+10 0.0 0.0" << true;
}

void ObjReaderTest::testParseNormalLine()
{
    QFETCH(QString, line);
    QFETCH(bool, expectedResult);

    Model model;
    bool result = parseSingleLine(line, model);

    QCOMPARE(result, expectedResult);

    if (expectedResult) {
        QVERIFY(!model.normals.isEmpty());
        qDebug() << "Parsed normals:" << model.normals.last();
    }
}

void ObjReaderTest::testParseFaceLine_data()
{
    QTest::addColumn<QString>("line");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("f v v v") << "f 1 2 3" << true;
    QTest::newRow("Extra data after") << "f 0 0 0" << false;
    QTest::newRow("f v/vt/vn") << "f 1/1/1 2/2/2 3/3/3" << true;
    QTest::newRow("f v//vn") << "f 1//1 2//2 3//3" << true;
    QTest::newRow("f v/vt") << "f 1/1 2/2 3/3" << true;
    QTest::newRow("Quad face") << "f 1 2 3 4" << true;
    QTest::newRow("Polygon face") << "f 1 2 3 4 5" << true;
    QTest::newRow("Face with 10 vertices") << "f 1 2 3 4 5 6 7 8 9 10" << true;
    QTest::newRow("Face with same indices") << "f 1 1 1" << true;
    QTest::newRow("Face with duplicate pairs") << "f 1/1/1 1/1/1 1/1/1" << true;
    QTest::newRow("f with leading/trailing spaces") << "1/1/1 2/2/2 3/-1/3 4/-1/4" << false;

    QTest::newRow("Only two vertices") << "f 1 2" << false;
    QTest::newRow("One vertex") << "f 1" << false;
    QTest::newRow("Empty face") << "f" << false;

    QTest::newRow("f with leading/trailing spaces") << "   f   1   2   3   " << true;
    //QTest::newRow("f with leading/trailing spaces") << "f 0 / 0 / 0 0 / 0 / 0 0 / 0 / 0 # no spaces" << true;
    QTest::newRow("f with leading/trailing spaces") << "f 0  /0/0 0/0/0 0/0/0  # no spaces" << false;
    QTest::newRow("f with leading/trailing spaces") << "f 0  / 0 / 0 0 / 0 / 0 0 / 0 / 0  # no spaces" << false;
    QTest::newRow("f with leading/trailing spaces") << "f 0  /0/0 0/ 0 /0 0/  0/  0  # no spaces" << false;
    QTest::newRow("f with leading/trailing spaces") << "f 0  /0/ 0 0/0/0 0/0/0  # no spaces" << false;
    QTest::newRow("f with tabs") << "f\t1\t2\t3" << true;
    QTest::newRow("With tab and space mix") << "f\t1 \t 2   \t3" << true;
    QTest::newRow("With carriage return") << "f 1 2 3\r" << true;
    QTest::newRow("With newline") << "f 1 2 3\n" << true;

    QTest::newRow("Trailing comment") << "f 1 2 3 #comment" << true;
    QTest::newRow("Face with multiple comments") << "f 1 2 3 #comment #another" << true;

    QTest::newRow("Invalid index") << "f abc 2 3" << false;
    QTest::newRow("Zero index") << "f 0 2 3" << false;
    QTest::newRow("Negative index") << "f -1 2 3" << false;
    QTest::newRow("Float index") << "f 1.5 2 3" << false;
    QTest::newRow("Index with letters") << "f 1a 2 3" << false;
    QTest::newRow("NaN index") << "f NaN 2 3" << false;
    QTest::newRow("Infinity index") << "f inf 2 3" << false;
    QTest::newRow("Very large index") << "f 999999999 999999999 999999999" << true;
    QTest::newRow("Max int index") << "f 2147483647 2147483647 2147483647" << true;
    QTest::newRow("Min int index") << "f -2147483647 2 3" << false;

    QTest::newRow("Mixed formats") << "f 1/2 2/3/4 3" << false;
    QTest::newRow("Face with extra slashes") << "f 1///1 2///2 3///3" << false;
    QTest::newRow("Face with empty parts") << "f 1// 2// 3//" << true;
    QTest::newRow("Face with missing vt") << "f 1/ 2/ 3/" << true;
    QTest::newRow("Face with missing vn") << "f 1// 2// 3//" << true;
    QTest::newRow("Empty part") << "f 1/2//4" << false;
    QTest::newRow("Face with extra slashes in one") << "f 1/2//3 2//3 3/4" << false;

    QTest::newRow("Extra data after") << "f 1 2 3 4 garbage" << false;
    QTest::newRow("Face with slash at end") << "f 1 2 3 /" << false;
    QTest::newRow("With hidden null char") << "f 1\0 2 3" << false;
    QTest::newRow("With quotes") << "f \"1\" 2 3" << false;
    QTest::newRow("With braces") << "f (1) {2} [3]" << false;
    QTest::newRow("Comma-separated") << "f 1,2,3" << false;
    QTest::newRow("Semicolon-separated") << "f 1;2;3" << false;
    QTest::newRow("Colon-separated") << "f 1:2:3" << false;
    QTest::newRow("Dotted format") << "f 1.2.3 2.3.4 3.4.5" << false;
    QTest::newRow("Face with underscores") << "f 1_0 2_0 3_0" << false;
    QTest::newRow("Face with units") << "f 1m 2cm 3km" << false;
    QTest::newRow("Face with currency") << "f $1 $2 $3" << false;
    QTest::newRow("Face with %") << "f 1% 2% 3%" << false;
    QTest::newRow("Face with binary numbers") << "f 0b10 0b11 0b100" << false;
    QTest::newRow("Face with hex numbers") << "f 0x1A 0x1B 0x1C" << false;

    QTest::newRow("Face with very long line") << "f 1 2 3 asdasdasdasdasdasdasdasdasdasdasdasdasdasdasd" << false;
    QTest::newRow("Some invalid index") << "f 1 abc 3" << false;
}

void ObjReaderTest::testParseFaceLine()
{
    QFETCH(QString, line);
    QFETCH(bool, expectedResult);

    Model model;
    QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int lineNum = 1;

    bool result = parseFace(tokens, line, model.faceVertexIndices,
                            model.faceTexCoordIndices, model.faceNormalIndices,
                            model.polygonStarts, model.polygonLengths, lineNum);

    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testValidateModel_data()
{
    QTest::addColumn<Model>("model");
    QTest::addColumn<bool>("expectedResult");

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(2);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        QTest::newRow("Valid triangle") << model << true;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(2);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        model.textureVertices.append(QVector2D(0.0f, 0.0f));
        model.textureVertices.append(QVector2D(1.0f, 0.0f));
        model.textureVertices.append(QVector2D(0.0f, 1.0f));

        model.faceTexCoordIndices.append(0);
        model.faceTexCoordIndices.append(1);
        model.faceTexCoordIndices.append(2);

        model.normals.append(QVector3D(0.0f, 0.0f, 1.0f));
        model.normals.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.normals.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceNormalIndices.append(0);
        model.faceNormalIndices.append(1);
        model.faceNormalIndices.append(2);

        QTest::newRow("Valid full model") << model << true;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(5);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        QTest::newRow("Index out of bounds") << model << false;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(-1);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        QTest::newRow("Negative index") << model << false;
    }

    {
        Model model;
        QTest::newRow("Empty model") << model << false;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        QTest::newRow("Only one vertex") << model << false;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(2);
        model.polygonStarts.append(0);
        model.polygonLengths.append(2);
        QTest::newRow("Polygon with less than 3 vertices") << model << false;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(2);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        model.textureVertices.append(QVector2D(0.0f, 0.0f));
        model.textureVertices.append(QVector2D(1.0f, 0.0f));
        model.textureVertices.append(QVector2D(0.0f, 1.0f));

        model.faceTexCoordIndices.append(0);
        model.faceTexCoordIndices.append(1);
        model.faceTexCoordIndices.append(5);
        QTest::newRow("Texture index out of bounds") << model << false;
    }

    {
        Model model;
        model.vertices.append(QVector3D(0.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.vertices.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceVertexIndices.append(0);
        model.faceVertexIndices.append(1);
        model.faceVertexIndices.append(2);
        model.polygonStarts.append(0);
        model.polygonLengths.append(3);

        model.normals.append(QVector3D(0.0f, 0.0f, 1.0f));
        model.normals.append(QVector3D(1.0f, 0.0f, 0.0f));
        model.normals.append(QVector3D(0.0f, 1.0f, 0.0f));

        model.faceNormalIndices.append(0);
        model.faceNormalIndices.append(1);
        model.faceNormalIndices.append(5);
        QTest::newRow("Normal index out of bounds") << model << false;
    }
}

void ObjReaderTest::testValidateModel()
{
    QFETCH(Model, model);
    QFETCH(bool, expectedResult);

    bool result = validateModel(model);
    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseTokens_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<int>("expectedVertices");
    QTest::addColumn<int>("expectedTexCoords");
    QTest::addColumn<int>("expectedNormals");
    QTest::addColumn<int>("expectedFaceIndices");

    QTest::newRow("Single vertex") << "v 1.0 2.0 3.0" << true << 1 << 0 << 0 << 0;
    QTest::newRow("Single texture coord") << "vt 0.5 0.5" << true << 0 << 1 << 0 << 0;
    QTest::newRow("Single normal") << "vn 0.0 1.0 0.0" << true << 0 << 0 << 1 << 0;
    QTest::newRow("Single face") << "f 1/1/1 2/2/2 3/3/3" << true << 0 << 0 << 0 << 3;

    QTest::newRow("Vertex with many spaces") << "v          1.0     \t   2.0   \v 3.0" << true << 1 << 0 << 0 << 0;
    QTest::newRow("Face with tabs and spaces") << "f\t1 \t 2   \t3" << true << 0 << 0 << 0 << 3;

    QTest::newRow("Line is comment") << "# This is a comment" << true << 0 << 0 << 0 << 0;
    QTest::newRow("Line with inline comment") << "v 1.0 2.0 3.0 # Inline comment" << true << 1 << 0 << 0 << 0;

    QTest::newRow("Empty line") << "" << true << 0 << 0 << 0 << 0;
    QTest::newRow("Whitespace-only line") << "    \t\t   " << true << 0 << 0 << 0 << 0;

    QTest::newRow("Unknown command") << "x 1.0 2.0 3.0" << true << 0 << 0 << 0 << 0;
    QTest::newRow("Invalid command format (no data)") << "v" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Invalid command with garbage") << "v garbage 2.0 3.0" << false << 0 << 0 << 0 << 0;

    QTest::newRow("Incomplete vertex") << "v 1.0 2.0" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Incomplete texture coord") << "vt 0.5" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Incomplete normal") << "vn 0.0 1.0" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Incomplete face") << "f 1/1/1 2/2/2" << false << 0 << 0 << 0 << 0;

    QTest::newRow("Vertex with scientific notation") << "v -1e+3 2.0 3.0" << true << 1 << 0 << 0 << 0;
    QTest::newRow("Vertex with decimal trailing") << "v 1. 2. 3." << true << 1 << 0 << 0 << 0;
    QTest::newRow("Vertex with invalid number") << "v 1.0 abc 3.0" << false << 0 << 0 << 0 << 0;

    QTest::newRow("Zero-indexed face") << "f 0 1 2" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Negative index in face") << "f -1 1 2" << false << 0 << 0 << 0 << 0;
    QTest::newRow("Face with missing vt") << "f 1/ 2/ 3/" << true << 0 << 0 << 0 << 0;
    QTest::newRow("Face with missing vn") << "f 1// 2// 3//" << true << 0 << 0 << 0 << 0;
    QTest::newRow("Face with extra slashes") << "f 1///1 2///2 3///3" << false << 0 << 0 << 0 << 0;
}

void ObjReaderTest::testParseTokens()
{
    QFETCH(QString, input);
    QFETCH(bool, expectedResult);

    Model model;
    QTextStream in(&input, QIODevice::ReadOnly);

    bool result = parseTokens(in, model);

    QCOMPARE(result, expectedResult);
}

void ObjReaderTest::testParseObj_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("expectedResult");


    QTest::newRow("Valid file") << "/home/r3ds/Internship/resources/cube.obj" << true;
    QTest::newRow("File not found") << ":/models/nonexistent.obj" << false;
}

void ObjReaderTest::testParseObj()
{
    QFETCH(QString, fileName);
    QFETCH(bool, expectedResult);

    Model model;
    bool result = parseObj(fileName, model);

    QCOMPARE(result, expectedResult);
}


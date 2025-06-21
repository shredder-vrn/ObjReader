#ifndef OBJREADERTEST_H
#define OBJREADERTEST_H

#include <QObject>

class ObjReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void testParseVertexLine_data();
    void testParseVertexLine();

    void testParseFaceLine1();
    void testParseFaceLine2();
    void testParseFaceLine3();
};

#endif // OBJREADERTEST_H

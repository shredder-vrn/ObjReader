#ifndef OBJREADERTEST_H
#define OBJREADERTEST_H

#include <QObject>

class ObjReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void testParseVertexLine_data();
    void testParseVertexLine();
    void testParseTexCoordLine_data();
    void testParseTexCoordLine();
    void testParseNormalLine_data();
    void testParseNormalLine();
    void testParseFaceLine_data();
    void testParseFaceLine();

};

#endif // OBJREADERTEST_H

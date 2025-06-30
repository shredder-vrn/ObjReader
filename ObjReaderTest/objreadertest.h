#ifndef OBJREADERTEST_H
#define OBJREADERTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include "Model/model.h"

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
    void testValidateModel_data();
    void testValidateModel();
    void testParseTokens_data();
    void testParseTokens();
    void testparseObj_data();
    void testparseObj();
};

#endif // OBJREADERTEST_H

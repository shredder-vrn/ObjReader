#ifndef MESHDATATEST_H
#define MESHDATATEST_H

#include <QObject>

class MeshDataTest : public QObject
{
    Q_OBJECT

private slots:
    void emptyFaceTest();
    void testValidFace();
    void testMismatchedTexCoordsCount();
    void testMismatchedNormalCount();
    void testIndexParsing_data();
    void testIndexParsing();
};

#endif // MESHDATATEST_H

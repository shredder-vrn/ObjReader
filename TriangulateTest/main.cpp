#include <QtTest/QtTest>
#include "TriangulateTest/triangulatetest.h"


//QTEST_APPLESS_MAIN(MeshDataTest)

int main()
{
    //MeshDataTest mdTest;
    //QTest::qExec(&mdTest);

    TriangulateTest orTest;
    QTest::qExec(&orTest);

    return 0;
}

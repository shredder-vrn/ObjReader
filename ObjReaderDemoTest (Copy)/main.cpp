#include <QtTest/QtTest>
#include "ObjReaderTest/objreadertest.h"


//QTEST_APPLESS_MAIN(MeshDataTest)

int main()
{
    //MeshDataTest mdTest;
    //QTest::qExec(&mdTest);

    ObjReaderTest orTest;
    QTest::qExec(&orTest);

    return 0;
}

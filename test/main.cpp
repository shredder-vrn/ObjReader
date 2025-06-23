#include <QtTest/QtTest>
#include "../src/obj_reader/modeldata.h"
#include "obj_reader/objreadertest.h"


//QTEST_APPLESS_MAIN(MeshDataTest)

int main()
{
    //MeshDataTest mdTest;
    //QTest::qExec(&mdTest);

    ObjReaderTest orTest;
    QTest::qExec(&orTest);

    return 0;
}

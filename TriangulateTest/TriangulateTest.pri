!TRIANGULATE_TEST {

CONFIG += TRIANGULATE_TEST

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Triangulate/Triangulate.pri)
include($$PWD/../ObjReader/ObjReader.pri)

HEADERS += \
    $$PWD/../TriangulateTest/triangulatetest.h
SOURCES += \
    $$PWD/../TriangulateTest/triangulatetest.cpp
}











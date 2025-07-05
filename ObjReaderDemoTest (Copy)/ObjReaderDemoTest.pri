!OBJ_READER_DEMO_TEST_PRI {

CONFIG += OBJ_READER_DEMO_TEST_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../ObjReaderDemo/ObjReaderDemo.pri)

HEADERS += \
    $$PWD/../ObjReaderTest/objreadertest.h
SOURCES += \
    $$PWD/../ObjReaderTest/objreadertest.cpp
}











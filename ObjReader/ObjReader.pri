!OBJ_READER_PRI {
CONFIG += OBJ_READER_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

include($$PWD/../Model/Model.pri)


HEADERS += \
    $$PWD/objreader.h

SOURCES += \
    $$PWD/objreader.cpp

}






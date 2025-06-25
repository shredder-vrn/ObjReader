!MODEL {
CONFIG += MODEL

INCLUDEPATH *= $$clean_path($$PWD/../)

#include($$PWD/../Model/Model.pri)
#include($$PWD/../ObjReader/ObjReader.pri)


SOURCES += \
    $$PWD/viewport.cpp

HEADERS += \
    $$PWD/viewport.h

}








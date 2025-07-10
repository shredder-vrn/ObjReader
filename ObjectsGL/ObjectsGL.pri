!OBJECTSGL_PRI {
CONFIG += OBJECTSGL_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/modelgl.h\
    $$PWD/objectsgl.h\
    $$PWD/gridgl.h


SOURCES += \
    $$PWD/modelgl.cpp\
    $$PWD/gridgl.cpp

}

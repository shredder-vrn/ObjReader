!OBJECTSGL_PRI {
CONFIG += OBJECTSGL_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/modelgl.h\

SOURCES += \
    $$PWD/modelgl.cpp\

}

HEADERS += \
    $$PWD/objectsgl.h

SOURCES += \
    $$PWD/objectsgl.cpp

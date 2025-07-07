!MODEL_PRI {
CONFIG += MODEL_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/modelgl.h\
    $$PWD/modeldata.h

SOURCES += \
    $$PWD/modelgl.cpp\
    $$PWD/modeldata.cpp

}

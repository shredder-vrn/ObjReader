!MODEL_PRI {
CONFIG += MODEL_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/model.h\
    $$PWD/modelgl.h


SOURCES += \
    $$PWD/model.cpp\
    $$PWD/modelgl.cpp

}


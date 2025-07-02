!SCENE {
CONFIG += SCENE

INCLUDEPATH *= $$clean_path($$PWD/../)

#REVIEW: include

HEADERS += \
    $$PWD/scene.h\
    $$PWD/scenerefactoring.h

SOURCES += \
    $$PWD/scene.cpp\
    $$PWD/scenerefactoring.cpp

}














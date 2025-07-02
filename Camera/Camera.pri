!CAMERA {
CONFIG += CAMERA

INCLUDEPATH *= $$clean_path($$PWD/../)


HEADERS += \
    $$PWD/camera.h\
    $$PWD/camerarefactoring.h

SOURCES += \
    $$PWD/camera.cpp\
    $$PWD/camerarefactoring.cpp

}





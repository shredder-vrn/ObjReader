!CAMERA_PRI {

CONFIG += CAMERA_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/camera.h \
    $$PWD/cameraorthographic.h \
    $$PWD/cameraperspective.h

SOURCES += \
    $$PWD/cameraorthographic.cpp \
    $$PWD/cameraperspective.cpp

}


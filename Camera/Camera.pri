!CAMERA_PRI {

CONFIG += CAMERA_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/cameraorthographic.h \
    $$PWD/cameraperspective.h \
    $$PWD/сamera.h

SOURCES += \
    $$PWD/camera.cpp\
    $$PWD/cameraorthographic.cpp \
    $$PWD/cameraperspective.cpp

}


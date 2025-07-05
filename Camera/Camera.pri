!CAMERA_PRI {

CONFIG += CAMERA_PRI

INCLUDEPATH *= $$clean_path($$PWD/../)

HEADERS += \
    $$PWD/cameraort.h \
    $$PWD/cameraper.h\
    $$PWD/сamerainterface.h

SOURCES += \
    $$PWD/camera.cpp\
    $$PWD/cameraort.cpp \
    $$PWD/cameraper.cpp \

}


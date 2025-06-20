QT -= gui
QT += core gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../ObjReader/ObjReader.pri)
include($$PWD/../ObjReaderTest/ObjReaderTest.pri)
# include($$PWD/../Model/Model.pri)
# include($$PWD/../ModelTest/ModelTest.pri)


SOURCES += \
        $$PWD/Main.cpp \
        $$PWD/foo.cpp

HEADERS += \
    $$PWD/foo.h

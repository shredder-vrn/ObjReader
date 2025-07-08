LIBS += -lGL
QT -= gui
QT += core gui
QT += testlib
QT += core gui widgets opengl
QT += opengl

QMAKE_CXXFLAGS += -std=c++17

CONFIG += c++17 console
CONFIG -= app_bundle

include($$PWD/../MainWindow/MainWindow.pri)

HEADERS += \
    logger.h\

SOURCES += \
        $$PWD/main.cpp\

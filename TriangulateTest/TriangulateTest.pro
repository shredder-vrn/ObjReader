QT -= gui
QT += core gui
QT += testlib
QT += core gui widgets opengl

CONFIG += c++11 console
CONFIG -= app_bundle

include($$PWD/../TriangulateTest/TriangulateTest.pri)

SOURCES += \
    $$PWD/main.cpp \

HEADERS += \


QT -= gui
QT += core gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

include($$PWD/../TriangulateTest/TriangulateTest.pri)

SOURCES += \
    $$PWD/main.cpp \

HEADERS += \


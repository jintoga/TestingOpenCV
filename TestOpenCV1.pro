QT += core
QT -= gui

TARGET = TestOpenCV1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += C:/Users/DAT/Desktop/opencv/build/include
LIBS += C:/Users/DAT/Desktop/opencv/cmake-debug/lib/*.a

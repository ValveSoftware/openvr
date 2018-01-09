#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui
QT       += core gui opengl
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloWorldOverlay
TEMPLATE = app


SOURCES += main.cpp\
        overlaywidget.cpp \
    openvroverlaycontroller.cpp

HEADERS  += overlaywidget.h \
    openvroverlaycontroller.h

FORMS    += overlaywidget.ui

INCLUDEPATH += ../../headers

win32:contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L../../lib/win64 -lopenvr_api
    DESTDIR = ../bin/win64
} else {
    LIBS += -L../../lib/win32 -lopenvr_api
    DESTDIR = ../bin/win32
}

unix:!macx {
LIBS += -L../../lib/win32 -lopenvr_api
    LIBS += -L../../lib/linux32 -lopenvr_api
    DESTDIR = ../bin/linux32
}

macx: {
    #QMAKE_CXXFLAGS += -std=c++11
    CONFIG += c++11
    LIBS += -L../../lib/osx32 -lopenvr_api
DESTDIR = ../bin/win32
    DESTDIR = ../bin/osx32
}


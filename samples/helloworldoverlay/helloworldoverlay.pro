#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui
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

LIBS += -L../../lib/win32 -lopenvr_api

DESTDIR = ../bin/win32

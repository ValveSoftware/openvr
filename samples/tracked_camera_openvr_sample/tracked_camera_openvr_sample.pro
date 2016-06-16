#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T14:37:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tracked_camera_openvr_sample
TEMPLATE = app


SOURCES += main.cpp\
        tracked_camera_openvr_sample.cpp

HEADERS  += tracked_camera_openvr_sample.h

INCLUDEPATH += ../../headers

LIBS += -L../../lib/win32 -lopenvr_api

DESTDIR = ../bin/win32

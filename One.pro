#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T11:40:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = One
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

DEPENDPATH += /usr/include

#INCLUDEPATH += /usr/include/gstreamer-1.0/
#INCLUDEPATH += /usr/include/glib-2.0/
#INCLUDEPATH += /usr/include/glib-2.0/glib/


INCLUDEPATH += /usr/include/
INCLUDEPATH += /usr/include/gstreamer-1.0/
INCLUDEPATH += /usr/include/gstreamer-1.0/gst/
INCLUDEPATH += /home/nvidia/tuanheWork/compile/install/include/glib-2.0/
INCLUDEPATH += /home/nvidia/tuanheWork/compile/install/include/glib-2.0/glib/
INCLUDEPATH += /home/nvidia/tuanheWork/compile/install/lib/glib-2.0/include/
INCLUDEPATH += /usr/include/libxml2/


unix:!macx:!symbian: LIBS +=  -lglib-2.0 \
-lgobject-2.0 -lxml2 -lgthread-2.0 -lgmodule-2.0 \
-lz \
-lgstreamer-1.0 -lgstvideo-1.0

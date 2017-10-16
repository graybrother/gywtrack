#-------------------------------------------------
#
# Project created by QtCreator 2017-10-16T14:01:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gywtrack
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    header.h \
    gyw.h

FORMS    += mainwindow.ui

INCLUDEPATH += c:/opencv-contrib/include/opencv \
                 c:/opencv-contrib/include/opencv2 \
                 c:/opencv-contrib/include

LIBS +=  c:/opencv-contrib/lib/libopencv_*.dll.a \

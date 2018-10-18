#-------------------------------------------------
#
# Project created by QtCreator 2018-10-18T23:29:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp

HEADERS  += mainwindow.h \
    openglwidget.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    shader.qrc \
    textures.qrc

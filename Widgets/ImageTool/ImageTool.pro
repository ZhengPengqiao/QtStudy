#-------------------------------------------------
#
# Project created by QtCreator 2018-10-12T23:58:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageTool
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    painterwidget.cpp \
    viewitem.cpp

HEADERS  += mainwindow.h \
    painterwidget.h \
    viewitem.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc

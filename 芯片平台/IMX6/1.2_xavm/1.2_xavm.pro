#-------------------------------------------------
#
# Project created by QtCreator 2017-10-13T14:26:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = xavm
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += main.cpp\
        cbackgroundwnd.cpp \
    clayersetup.cpp

HEADERS  += cbackgroundwnd.h \
    clayersetup.h

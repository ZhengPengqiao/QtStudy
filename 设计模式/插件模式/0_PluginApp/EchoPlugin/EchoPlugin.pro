TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets

INCLUDEPATH    += ../MainWindow
TARGET          = $$qtLibraryTarget(plugindemo)
DESTDIR         = ../plugins

HEADERS += \
    plugindemo.h

SOURCES += \
    plugindemo.cpp

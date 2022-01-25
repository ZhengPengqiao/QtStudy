#-------------------------------------------------
#
# Project created by QtCreator 2018-10-18T23:29:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hellogles3
TEMPLATE = app


SOURCES = glwindow.cpp \
          main.cpp

HEADERS = glwindow.h

DISTFILES +=

RESOURCES += \
    shader.qrc \
    textures.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogles3
INSTALLS += target

DISTFILES +=
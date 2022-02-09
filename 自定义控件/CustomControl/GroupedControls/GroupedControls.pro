CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(GroupedControls)
TEMPLATE    = lib

HEADERS     = ImageItemplugin.h TextItemplugin.h GroupedControls.h
SOURCES     = ImageItemplugin.cpp TextItemplugin.cpp GroupedControls.cpp
RESOURCES   = icon/icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

test.path = $$PWD/../CustomControl/TestControls/Plugin
qtdesigner.path = /opt/Qt5.9.5/Tools/QtCreator/lib/Qt/plugins/designer/
INSTALLS    += test qtdesigner

include(TextItem/TextItem.pri)
include(ImageItem/ImageItem.pri)

CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(GroupedControls)
TEMPLATE    = lib

HEADERS     =   GroupedControls.h

SOURCES     =   GroupedControls.cpp

RESOURCES   = icon/icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(TextItem/TextItem.pri)
include(ImageItem/ImageItem.pri)

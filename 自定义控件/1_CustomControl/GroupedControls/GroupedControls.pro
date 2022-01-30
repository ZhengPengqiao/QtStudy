CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(groupedcontrolsplugin)
TEMPLATE    = lib

HEADERS     = GroupedControlsplugin.h \
    GroupedControls.h
SOURCES     = GroupedControlsplugin.cpp \
    GroupedControls.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(GroupedControls.pri)

FORMS += \
    GroupedControls.ui

#-------------------------------------------------
#
# Project created by QtCreator 2012-12-18T00:30:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReadVideo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videoobj.cpp

HEADERS  += mainwindow.h \
    videoobj.h

FORMS    += mainwindow.ui
unix: LIBS += -L$$PWD/../../Doc/opencv-3.4.0_install/lib/
unix: INCLUDEPATH += $$PWD/../../Doc/opencv-3.4.0_install/include
unix: DEPENDPATH += $$PWD/../../Doc/opencv-3.4.0_install/include

unix: LIBS += -lopencv_dnn
unix: LIBS += -lopencv_ml
unix: LIBS += -lopencv_objdetect
unix: LIBS += -lopencv_shape
unix: LIBS += -lopencv_stitching
unix: LIBS += -lopencv_superres
unix: LIBS += -lopencv_videostab
unix: LIBS += -lopencv_calib3d
unix: LIBS += -lopencv_features2d
unix: LIBS += -lopencv_highgui
unix: LIBS += -lopencv_videoio
unix: LIBS += -lopencv_imgcodecs
unix: LIBS += -lopencv_video
unix: LIBS += -lopencv_photo
unix: LIBS += -lopencv_imgproc
unix: LIBS += -lopencv_flann
unix: LIBS += -lopencv_core


win32: LIBS += -L$$PWD/../../Doc/opencv-3.4.0_install_win_mingw32/x86/mingw/bin/
win32: INCLUDEPATH += $$PWD/../../Doc/opencv-3.4.0_install_win_mingw32/include
win32: DEPENDPATH += $$PWD/../../Doc/opencv-3.4.0_install_win_mingw32/include

win32: LIBS += -llibopencv_dnn340
win32: LIBS += -llibopencv_ml340
win32: LIBS += -llibopencv_objdetect340
win32: LIBS += -llibopencv_shape340
win32: LIBS += -llibopencv_stitching340
win32: LIBS += -llibopencv_superres340
win32: LIBS += -llibopencv_videostab340
win32: LIBS += -llibopencv_calib3d340
win32: LIBS += -llibopencv_features2d340
win32: LIBS += -llibopencv_highgui340
win32: LIBS += -llibopencv_videoio340
win32: LIBS += -llibopencv_imgcodecs340
win32: LIBS += -llibopencv_video340
win32: LIBS += -llibopencv_photo340
win32: LIBS += -llibopencv_imgproc340
win32: LIBS += -llibopencv_flann340
win32: LIBS += -llibopencv_core340

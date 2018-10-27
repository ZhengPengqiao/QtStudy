HEADERS = glwindow.h

SOURCES = glwindow.cpp \
          main.cpp

RESOURCES += \
    shader.qrc \
    textures.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogles3
INSTALLS += target

DISTFILES +=

include(qtcanvas.files)

ROOT_DIRECTORY  = $$PWD
BIN_DIRECTORY   = $${ROOT_DIRECTORY}/bin
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
DESTDIR         = $${BIN_DIRECTORY}
OBJECTS_DIR     = $${BUILD_DIRECTORY}/$${TARGET}
MOC_DIR         = $${BUILD_DIRECTORY}/$${TARGET}

VERSION = 1.0.0

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBPATH += /usr/local/lib
    INCLUDEPATH += /usr/local/include # for libcanvas / libjsoncpp

    ICON = qtCanvas/resources/icons/canvas.icns
}

win32 {
  RC_FILE = qtcanvas.rc

  CONFIG(release, debug|release) {
    LIBS += -lcanvas
  }
}

unix {
  LIBS += -lcanvas -ljsoncpp
}
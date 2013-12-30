include(qtCanvas.files)

ROOT_DIRECTORY  = $$PWD
BIN_DIRECTORY   = $${ROOT_DIRECTORY}/bin
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
DESTDIR         = $${BIN_DIRECTORY}
OBJECTS_DIR     = $${BUILD_DIRECTORY}/$${TARGET}
MOC_DIR         = $${BUILD_DIRECTORY}/$${TARGET}

VERSION = 1.0.1

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++0x

macx {
    BUNDLE_DIR = $${BIN_DIRECTORY}/$${TARGET}.app

    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBPATH += /usr/local/lib
    INCLUDEPATH += /usr/local/include # for libcanvas / libjsoncpp

    ICON = $$PWD/resources/icons/canvas.icns
    QMAKE_INFO_PLIST = $$PWD/resources/Info.plist
    QMAKE_INFO_PLIST_OUT = $${BUNDLE_DIR}/Contents/Info.plist

    PRE_TARGETDEPS += $${BIN_DIRECTORY}

    missing.target = dummy
    missing.depends = $${BUNDLE_DIR}/Contents/Info.plist $${BUNDLE_DIR}/Contents/Resources/canvas.icns

    QMAKE_EXTRA_TARGETS = missing
    QMAKE_PRE_LINK = make dummy
}

win32 {
  RC_FILE = qtCanvas.rc

  CONFIG(release, debug|release) {
    LIBS += -lcanvas
  }
}

unix {
  LIBS += -lcanvas -ljsoncpp
}

HEADERS += \
    include/async_session.hpp

SOURCES += \
    src/async_session.cpp

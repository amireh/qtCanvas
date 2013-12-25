ROOT_DIRECTORY = $$PWD
BIN_DIRECTORY = $${ROOT_DIRECTORY}/bin
BUILD_DIRECTORY = $${ROOT_DIRECTORY}/build
TESTS_DIRECTORY = $${BUILD_DIRECTORY}/unit_tests
EXAMPLES_DIRECTORY = $${BUILD_DIRECTORY}/examples

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DESTDIR = $${BIN_DIRECTORY}
OBJECTS_DIR = $${BUILD_DIRECTORY}/$${TARGET}
MOC_DIR = $${BUILD_DIRECTORY}/$${TARGET}

QMAKE_CXXFLAGS += -std=c++0x

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
    QMAKE_CXXFLAGS += -stdlib=libc++
    LIBPATH += /usr/local/lib
    INCLUDEPATH += /usr/local/include # for libcanvas / libjsoncpp
}

INCLUDEPATH += $${ROOT_DIRECTORY}/qtCanvas/include
FILES = $$cat($${ROOT_DIRECTORY}/qtCanvas/qtCanvas.files)

HEADERS = $$find(FILES, "include/.*\.h(pp|xx)?")
HEADERS = $$replace(HEADERS, "include","$${ROOT_DIRECTORY}/qtCanvas/include")
HEADERS = $$unique(HEADERS)

SOURCES = $$find(FILES, "src/.*\.c(pp|xx)?")
SOURCES = $$replace(SOURCES, "src","$${ROOT_DIRECTORY}/qtCanvas/src")
SOURCES = $$unique(SOURCES)

RESOURCES = $${ROOT_DIRECTORY}/qtCanvas/resources/qtCanvas.qrc

FORMS = $$find(FILES, "forms/.*\.ui")
FORMS = $$replace(FORMS, "forms","$${ROOT_DIRECTORY}/qtCanvas/forms")
FORMS = $$unique(FORMS)

win32:CONFIG(release, debug|release): LIBS += -lcanvas
else:win32:CONFIG(debug, debug|release): LIBS += -lcanvas
else:unix: LIBS += -lcanvas -ljsoncpp

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

HEADERS  += \
    $${ROOT_DIRECTORY}/qtCanvas/include/api_delegate.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/mainwindow.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/available_quizzes.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/login.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/state.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/viewport.h \
    $${ROOT_DIRECTORY}/qtCanvas/include/type_exports.hpp \
    $${ROOT_DIRECTORY}/qtCanvas/include/qproxy.hpp \
    $${ROOT_DIRECTORY}/qtCanvas/include/take_quiz.hpp \
    $${ROOT_DIRECTORY}/qtCanvas/include/qview.hpp

SOURCES += \
    $${ROOT_DIRECTORY}/qtCanvas/src/mainwindow.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/available_quizzes.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/login.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/state.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/viewport.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/qproxy.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/take_quiz.cpp \
    $${ROOT_DIRECTORY}/qtCanvas/src/qview.cpp

FORMS += \
  $${ROOT_DIRECTORY}/qtCanvas/forms/mainwindow.ui \
  $${ROOT_DIRECTORY}/qtCanvas/forms/available_quizzes.ui \
  $${ROOT_DIRECTORY}/qtCanvas/forms/login.ui \
  $${ROOT_DIRECTORY}/qtCanvas/forms/take_quiz.ui

win32:CONFIG(release, debug|release): LIBS += -lcanvas
else:win32:CONFIG(debug, debug|release): LIBS += -lcanvas
else:unix: LIBS += -lcanvas -ljsoncpp

include(../buildInfo.pri)

QT += testlib

TEMPLATE = app
TARGET = qtCanvasTests

CONFIG += testcase
CONFIG -= app_bundle
CONFIG += console

QMAKE_CXXFLAGS += -DQTCANVAS_TEST

INCLUDEPATH += \
  $${ROOT_DIRECTORY}/qtCanvas \
  $${BUILD_DIRECTORY}/qtCanvas # for moc'ed UI headers

DEPENDPATH += \
  $${BUILD_DIRECTORY} \
  $${BUILD_DIRECTORY}/qtCanvas # for .o files

HEADERS += tests.hpp
SOURCES -= $${ROOT_DIRECTORY}/qtCanvas/src/main.cpp
SOURCES += tests.cpp

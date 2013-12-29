#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T12:03:16
#
#-------------------------------------------------
include(../buildInfo.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0

TARGET = qtCanvas
TEMPLATE = app

HEADERS += \
    include/views/take_quiz_oqaat.hpp \
    ext/include/QRadioButton.hpp \
    include/ext/QRadioButton.hpp

SOURCES += \
    src/views/take_quiz_oqaat.cpp \
    src/ext/QRadioButton.cpp

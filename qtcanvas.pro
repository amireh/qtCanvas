#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T12:03:16
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtCanvas
TEMPLATE = app


SOURCES +=\
    src/mainwindow.cpp \
    src/main.cpp \
    src/available_quizzes.cpp \
    src/login.cpp \
    src/state.cpp \
    src/viewport.cpp \
    src/qproxy.cpp \
    src/take_quiz.cpp \
    src/qview.cpp

HEADERS  += \
    include/api_delegate.h \
    include/mainwindow.h \
    include/available_quizzes.h \
    include/login.h \
    include/state.h \
    include/viewport.h \
    include/type_exports.hpp \
    include/qproxy.hpp \
    include/take_quiz.hpp \
    include/qview.hpp

FORMS    += forms/mainwindow.ui \
    forms/available_quizzes.ui \
    forms/login.ui \
    forms/take_quiz.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../libcanvas/lib/release/ -lcanvas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../libcanvas/lib/debug/ -lcanvas
else:unix: LIBS += -L$$PWD/../../libcanvas/lib/ -lcanvas

INCLUDEPATH += $$PWD/../../libcanvas/include
DEPENDPATH += $$PWD/../../libcanvas/include

OTHER_FILES +=

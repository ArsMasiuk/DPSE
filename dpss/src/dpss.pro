#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T16:34:51
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dpss
TEMPLATE = app

COMPILER_NAME = unknown
g++*{
    COMPILER_NAME = gcc
}
mingw*{
    COMPILER_NAME = mingw
}
msvc*{
    COMPILER_NAME = msvc
}

DESTDIR = $$PWD/../bin.$$COMPILER_NAME
CONFIG(debug, debug|release):DESTDIR = $$DESTDIR"_debug"

include(qvge/qvge.pri)
include(simu/mpisch/mpisch.pri)

INCLUDEPATH += qvge ds common simu

SOURCES += $$files($$PWD/ds/*.cpp)
HEADERS  += $$files($$PWD/ds/*.h) \
    simu/ilogger.h
FORMS    += $$files($$PWD/ds/*.ui)
#RESOURCES += $$files($$PWD/ds/*.qrc)

SOURCES += $$files($$PWD/common/*.cpp)
HEADERS  += $$files($$PWD/common/*.h)
FORMS    += $$files($$PWD/common/*.ui)
#RESOURCES += $$files($$PWD/common/*.qrc)

HEADERS  += $$files($$PWD/simu/*.h)

LIBS += -lopengl32 -lGlu32

g++{
    QMAKE_CXXFLAGS += -fpermissive
}
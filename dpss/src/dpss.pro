#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T16:34:51
#
#-------------------------------------------------

QT       += core gui xml opengl 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts printsupport

TARGET = dpss
TEMPLATE = app

CONFIG += c++11

COMPILER_NAME = unknown

g++*{
    COMPILER_NAME = gcc
    QMAKE_CXXFLAGS += -fpermissive --std=C11
    DEFINES += __STDC_WANT_LIB_EXT1__
}

mingw*{
    COMPILER_NAME = mingw
}

msvc*{
    COMPILER_NAME = msvc
    QMAKE_CXXFLAGS -= -Zc:strictStrings
}

DESTDIR = $$PWD/../bin.$$COMPILER_NAME
CONFIG(debug, debug|release):DESTDIR = $$DESTDIR"_debug"

include(common/common.pri)
include(qvge/qvge.pri)
include(simu/mpisch/mpisch.pri)
include(simu/graph_om/graph_om.pri)
include(ui/ui.pri)

INCLUDEPATH += qvge ds common simu ui

SOURCES += $$files($$PWD/ds/*.cpp)
HEADERS  += $$files($$PWD/ds/*.h)
FORMS    += $$files($$PWD/ds/*.ui)
RESOURCES += $$files($$PWD/ds/*.qrc)

HEADERS  += $$files($$PWD/simu/*.h)

win32{
    LIBS += -lopengl32 -lGlu32
}


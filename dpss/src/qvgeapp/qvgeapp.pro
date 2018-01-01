# This file is a part of
# QVGE - Qt Visual Graph Editor
#
# (c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)
#
# It can be used freely, maintaining the information above.


TEMPLATE = app
TARGET = qvgeapp

QT += core gui widgets xml opengl network printsupport
CONFIG += c++11

SOURCES += $$files($$PWD/src/*.cpp)
HEADERS += $$files($$PWD/src/*.h)
FORMS += $$files($$PWD/src/*.ui)
RESOURCES += $$files($$PWD/src/*.qrc)

include($$PWD/base/base.pri)
include($$PWD/../qvge/qvge.pri)

SOURCES += $$files($$PWD/3rdParty/qsint-widgets/*.cpp)
HEADERS += $$files($$PWD/3rdParty/qsint-widgets/*.h)

include($$PWD/3rdParty/qsint-properties/PropertyEditor.pri)

INCLUDEPATH += $$PWD $$PWD/.. $$PWD/src $$PWD/3rdParty/qsint-widgets $$PWD/3rdParty/qsint-properties

win32{
        LIBS += -lopengl32 -lglu32 -lshell32 -luser32 -lpsapi
}

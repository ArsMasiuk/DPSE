# This file is a part of
# DPSE - Distributed Parallel Simulation Environment
#
# (c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)
#
# It can be used freely, maintaining the information above.


TEMPLATE = app
TARGET = dpseapp
VERSION = 2.1.0.0

include(../config.pri)
include(../app.pri)

QT += charts

# app sources
SOURCES += $$files($$PWD/*.cpp) $$files($$PWD/simu/branch/*.cpp)
HEADERS += $$files($$PWD/*.h) $$files($$PWD/simu/branch/*.h)
FORMS += $$files($$PWD/*.ui) $$files($$PWD/simu/branch/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

SOURCES += $$files($$PWD/simu/*.cpp)
HEADERS += $$files($$PWD/simu/*.h)

# includes & libs
INCLUDEPATH += $$PWD $$PWD/.. $$PWD/../3rdParty/qtpropertybrowser $$PWD/../3rdParty/qsint-widgets

LIBS += -lqtpropertybrowser -lqsint-widgets -lqvge -lqvgeio -lcommonui

USE_OGDF{
    LIBS += -logdf
}

win32{
    LIBS += -lopengl32 -lglu32 -lshell32 -luser32 -lpsapi

    RC_FILE = $$PWD/win32/icon.rc
}

cygwin*{
    LIBS += -lopengl32 -lglu32 -lshell32 -luser32 -lpsapi
}


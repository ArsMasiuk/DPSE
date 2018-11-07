# This file is a part of
# QVGE - Qt Visual Graph Editor
#
# (c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)
#
# It can be used freely, maintaining the information above.


TEMPLATE = app
TARGET = qvgeapp
VERSION = 0.5.0.0


include(../config.pri)
include(../app.pri)


# app sources
SOURCES += $$files($$PWD/*.cpp)
HEADERS += $$files($$PWD/*.h)
FORMS += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)


# includes & libs
INCLUDEPATH += $$PWD $$PWD/.. $$PWD/../3rdParty/qtpropertybrowser $$PWD/../3rdParty/qsint-widgets

LIBS += -lcommonui -lqvge -lqvgeio -lqtpropertybrowser -lqsint-widgets

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


# install
target.path = /usr/local/bin
INSTALLS += target


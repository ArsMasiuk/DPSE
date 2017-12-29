TEMPLATE = app
TARGET = qvgeapp

QT += core gui widgets xml opengl network printsupport
CONFIG += c++11

SOURCES += $$files($$PWD/*.cpp)
HEADERS  += $$files($$PWD/*.h)

include($$PWD/../base/base.pri)
include($$PWD/../common/common.pri)
include($$PWD/../ui/ui.pri)
include($$PWD/../qvge/qvge.pri)

INCLUDEPATH += $$PWD/..

win32{
        LIBS += -lopengl32 -lglu32 -lshell32 -luser32 -lpsapi
}

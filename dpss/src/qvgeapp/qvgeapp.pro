TEMPLATE = app
TARGET = qvgeapp

QT += core gui widgets xml opengl

SOURCES += $$files($$PWD/*.cpp)
HEADERS  += $$files($$PWD/*.h)

include($$PWD/../base/base.pri)
include($$PWD/../qvge/qvge.pri)

INCLUDEPATH += $$PWD/..

win32{
LIBS += -lopengl32 -lglu32
}

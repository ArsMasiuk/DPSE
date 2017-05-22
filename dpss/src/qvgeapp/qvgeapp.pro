TEMPLATE = app
TARGET = qvge

QT += core gui widgets xml

SOURCES += $$files($$PWD/*.cpp)
HEADERS  += $$files($$PWD/*.h)

include($$PWD/../base/base.pri)
include($$PWD/../qvge/qvge.pri)

INCLUDEPATH += $$PWD/..

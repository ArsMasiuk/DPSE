#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T16:34:51
#
#-------------------------------------------------

QT       += core gui xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BranchEditor3
TEMPLATE = app

include(qvge/qvge.pri)
INCLUDEPATH += qvge

SOURCES += $$files($$PWD/*.cpp)

HEADERS  += $$files($$PWD/*.h)

FORMS    += $$files($$PWD/*.ui)

RESOURCES += $$files($$PWD/*.qrc)

LIBS += -lopengl32 -lGlu32


QMAKE_CXXFLAGS += -fpermissive

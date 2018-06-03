QT += core gui widgets xml opengl network printsupport
CONFIG += c++11

# app sources
SOURCES += $$files($$PWD/*.cpp)
HEADERS += $$files($$PWD/*.h)
FORMS += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

# base sources
include($$PWD/../appbase/appbase.pri)
#include($$PWD/../qvge/qvge.pri)

# includes 
INCLUDEPATH += $$PWD $$PWD/.. 
INCLUDEPATH += $$PWD/../3rdParty/qsint-widgets 
INCLUDEPATH += $$PWD/../3rdParty/qtpropertybrowser 
INCLUDEPATH += $$PWD/../3rdParty/ogdf/include

include(../lib.pri)

CONFIG(debug, debug|release){
        DESTDIR = $$OUT_PWD/../lib.debug
}
else{
        DESTDIR = $$OUT_PWD/../lib
}

TARGET = commonui
QT += core gui widgets printsupport xml

SOURCES += $$files($$PWD/*.cpp)
HEADERS += $$files($$PWD/*.h)
FORMS += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

INCLUDEPATH += $$PWD $$PWD/.. $$PWD/../3rdParty/qtpropertybrowser $$PWD/../3rdParty/qsint-widgets

include($$PWD/../appbase/appbase.pri)

SOURCES += $$files($$PWD/*.cpp) $$files($$PWD/3rdParty/*.cpp)
HEADERS += $$files($$PWD/*.h) $$files($$PWD/3rdParty/*.h)
FORMS += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

INCLUDEPATH += $$PWD $$PWD/3rdParty

unix{
    SOURCES += $$PWD/3rdParty/readproc/read_proc.c
    SOURCES += $$PWD/3rdParty/readproc/struct.c
    HEADERS += $$files($$PWD/3rdParty/readproc/*.h)

    INCLUDEPATH += $$PWD/3rdParty/readproc
}


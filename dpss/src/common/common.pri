SOURCES += $$files($$PWD/*.cpp) $$files($$PWD/3rdParty/*.cpp)
HEADERS += $$files($$PWD/*.h) $$files($$PWD/3rdParty/*.h)
FORMS += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

INCLUDEPATH += $$PWD $$PWD/3rdParty

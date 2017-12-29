SOURCES += $$files($$PWD/*.cpp)
HEADERS  += $$files($$PWD/*.h)
#FORMS    += $$files($$PWD/*.ui)
RESOURCES += $$files($$PWD/*.qrc)

unix{
    QT += x11extras
    LIBS += -lX11
}

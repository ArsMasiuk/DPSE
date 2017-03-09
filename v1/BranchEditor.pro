#-------------------------------------------------
#
# Project created by QtCreator 2016-06-05T16:34:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BranchEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CEditorScene.cpp \
    CNode.cpp \
    CEditorView.cpp \
    CConnection.cpp \
    CBranchEditorView.cpp

HEADERS  += mainwindow.h \
    CEditorScene.h \
    CNode.h \
    CEditorView.h \
    CConnection.h \
    CBranchEditorView.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -fpermissive


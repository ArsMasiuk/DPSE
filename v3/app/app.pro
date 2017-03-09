TEMPLATE = app
TARGET = main

QT += core gui widgets

HEADERS += CMainWindow.h \
    CTabController.h \
    IDocument.h \
    CTextDocument.h \
    CDocumentController.h
SOURCES += main.cpp \
    CMainWindow.cpp \
    CTabController.cpp \
    CTextDocument.cpp \
    CDocumentController.cpp

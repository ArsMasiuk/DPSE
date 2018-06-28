# This file is a part of
# QVGE - Qt Visual Graph Editor
#
# (c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)
#
# It can be used freely, maintaining the information above.


TEMPLATE = subdirs
CONFIG += ordered

include($$PWD/config.pri)

USE_OGDF{
    SUBDIRS += ogdf
    ogdf.file = $$PWD/3rdParty/ogdf/ogdf.pro
}

SUBDIRS += qtpropertybrowser
qtpropertybrowser.file = $$PWD/3rdParty/qtpropertybrowser/qtpropertybrowser.pro

SUBDIRS += qsint
qsint.file = $$PWD/3rdParty/qsint-widgets/qsint-widgets.pro

SUBDIRS += qvge
qvge.file = $$PWD/qvge/qvge.pro

SUBDIRS += qvgeapp
qvgeapp.file = $$PWD/qvgeapp/qvgeapp.pro

SUBDIRS += commonui
commonui.file = $$PWD/commonui/commonui.pro


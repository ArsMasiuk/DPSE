/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QIcon>
#include <QKeySequence>

#include "CEditorSceneActions.h"
#include "CEditorScene.h"
#include "CNode.h"
#include "CEdge.h"


CEditorSceneActions::CEditorSceneActions(CEditorScene *scene)
{
	cutAction = new QAction(QIcon(":/Icons/Cut"), tr("Cu&t"), scene);
	cutAction->setStatusTip(tr("Cut selected item(s) to clipboard"));
	cutAction->setToolTip(tr("Cut selection"));
	cutAction->setShortcut(QKeySequence::Cut);
	connect(cutAction, &QAction::triggered, scene, &CEditorScene::cut);

	copyAction = new QAction(QIcon(":/Icons/Copy"), tr("&Copy"), scene);
	copyAction->setStatusTip(tr("Copy selected item(s) to clipboard"));
	copyAction->setToolTip(tr("Copy selection"));
	copyAction->setShortcut(QKeySequence::Copy);
	connect(copyAction, &QAction::triggered, scene, &CEditorScene::copy);

	pasteAction = new QAction(tr("Paste In Place"), scene);
	pasteAction->setStatusTip(tr("Paste item(s) from clipboard with the same coordinates"));
	pasteAction->setToolTip(tr("Paste from clipboard in-place"));
	connect(pasteAction, &QAction::triggered, scene, &CEditorScene::paste);

	delAction = new QAction(QIcon(":/Icons/Delete"), QObject::tr("&Delete"), scene);
	delAction->setStatusTip(QObject::tr("Delete selected item(s)"));
	delAction->setToolTip(QObject::tr("Delete selection"));
	delAction->setShortcut(QKeySequence::Delete);
	QObject::connect(delAction, &QAction::triggered, scene, &CEditorScene::del);
}

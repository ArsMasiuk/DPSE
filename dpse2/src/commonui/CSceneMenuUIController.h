#pragma once

#include <QObject>

#include <qvge/ISceneMenuController.h>


class CSceneMenuUIController : public QObject, public ISceneMenuController
{
	Q_OBJECT

public:
	CSceneMenuUIController(QObject *parent = nullptr);
	virtual ~CSceneMenuUIController();

	// reimp: ISceneMenuController
	virtual bool exec(CEditorScene *scene, QGraphicsItem *triggerItem, QGraphicsSceneContextMenuEvent *contextMenuEvent);
};



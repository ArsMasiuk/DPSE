#pragma once

#include <QObject>

#include <base/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>


class qvgeNodeEditorUIController : public QObject 
{
	Q_OBJECT

public:
	qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view);
	~qvgeNodeEditorUIController();

private:
	
};

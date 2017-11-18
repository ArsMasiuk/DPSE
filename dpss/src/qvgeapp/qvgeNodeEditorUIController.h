#pragma once

#include <QAction>

#include <base/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>


class qvgeNodeEditorUIController : public QObject 
{
	Q_OBJECT

public:
	qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view);
	~qvgeNodeEditorUIController();

public Q_SLOTS:
	void onSelectionChanged();
	
	void onZoomChanged(double currentZoom);
	void zoom();
	void unzoom();
	void resetZoom();

protected:
	CNodeEditorScene *m_scene;
	CEditorView *m_editorView;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *delAction;

	QAction *zoomAction;
	QAction *unzoomAction;
	QAction *resetZoomAction, *resetZoomAction2;
	QAction *fitZoomAction;
};

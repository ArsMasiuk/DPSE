#pragma once

#include <QAction>
#include <QLabel>

#include <base/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>

#include <slider2d.h>


class qvgeNodeEditorUIController : public QObject 
{
	Q_OBJECT

public:
	qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view);
	~qvgeNodeEditorUIController();

private Q_SLOTS:
	void exportFile();
	void exportPDF();

	void onSelectionChanged();
    void onSceneChanged();

	void onZoomChanged(double currentZoom);
	void zoom();
	void unzoom();
	void resetZoom();

    void sceneOptions();

    void onNavigatorShown();

private:
	void createMenus();
	void createPanels();
    void createNavigator();

private:
	CMainWindow *m_parent;
	CNodeEditorScene *m_scene;
	CEditorView *m_editorView;

    class QSint::Slider2d *m_sliderView;

    QLabel *m_statusLabel;

	QAction *cutAction;
	QAction *copyAction;
	QAction *pasteAction;
	QAction *delAction;
	QAction *unlinkAction;

	QAction *zoomAction;
	QAction *unzoomAction;
	QAction *resetZoomAction, *resetZoomAction2;
	QAction *fitZoomAction;

    QAction *gridAction;
    QAction *gridSnapAction;
    QAction *actionShowLabels;
};

#include "qvgeNodeEditorUIController.h"

#include <QMenuBar>


qvgeNodeEditorUIController::qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view) : QObject(parent),
	m_scene(scene), m_editorView(view)
{
	// connect scene
	connect(scene, &CEditorScene::sceneChanged, parent, &CMainWindow::onDocumentChanged);
	connect(scene, &CEditorScene::selectionChanged, this, &qvgeNodeEditorUIController::onSelectionChanged);

	// add edit menu
	QMenu *editMenu = new QMenu(tr("&Edit"));
	parent->menuBar()->insertMenu(parent->getWindowMenuAction(), editMenu);

	QAction *undoAction = editMenu->addAction(QIcon(":/Icons/Undo"), tr("&Undo"));
	undoAction->setStatusTip(tr("Undo latest action"));
	undoAction->setShortcut(QKeySequence::Undo);
	connect(undoAction, &QAction::triggered, scene, &CEditorScene::undo);
	connect(scene, &CEditorScene::undoAvailable, undoAction, &QAction::setEnabled);
	undoAction->setEnabled(scene->availableUndoCount());

	QAction *redoAction = editMenu->addAction(QIcon(":/Icons/Redo"), tr("&Redo"));
	redoAction->setStatusTip(tr("Redo latest action"));
	redoAction->setShortcut(QKeySequence::Redo);
	connect(redoAction, &QAction::triggered, scene, &CEditorScene::redo);
	connect(scene, &CEditorScene::redoAvailable, redoAction, &QAction::setEnabled);
	redoAction->setEnabled(scene->availableRedoCount());

	editMenu->addSeparator();

	cutAction = editMenu->addAction(QIcon(":/Icons/Cut"), tr("Cu&t"));
	cutAction->setStatusTip(tr("Cut selection to clipboard"));
	cutAction->setShortcut(QKeySequence::Cut);
	connect(cutAction, &QAction::triggered, scene, &CEditorScene::cut);

	copyAction = editMenu->addAction(QIcon(":/Icons/Copy"), tr("&Copy"));
	copyAction->setStatusTip(tr("Copy selection to clipboard"));
	copyAction->setShortcut(QKeySequence::Copy);
	connect(copyAction, &QAction::triggered, scene, &CEditorScene::copy);

	pasteAction = editMenu->addAction(QIcon(":/Icons/Paste"), tr("&Paste"));
	pasteAction->setStatusTip(tr("Paste selection from clipboard"));
	pasteAction->setShortcut(QKeySequence::Paste);
	connect(pasteAction, &QAction::triggered, scene, &CEditorScene::paste);

	delAction = editMenu->addAction(QIcon(":/Icons/Delete"), tr("&Delete"));
	delAction->setStatusTip(tr("Delete selection"));
	delAction->setShortcut(QKeySequence::Delete);
	connect(delAction, &QAction::triggered, scene, &CEditorScene::del);

	// add edit toolbar
	QToolBar *editToolbar = parent->addToolBar(tr("Edit"));
	editToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	
	editToolbar->addAction(undoAction);
	editToolbar->addAction(redoAction);
	
	editToolbar->addSeparator();

	editToolbar->addAction(cutAction);
	editToolbar->addAction(copyAction);
	editToolbar->addAction(pasteAction);
	editToolbar->addAction(delAction);


	// add view menu
	QMenu *viewMenu = new QMenu(tr("&View"));
	parent->menuBar()->insertMenu(parent->getWindowMenuAction(), viewMenu);

	zoomAction = viewMenu->addAction(QIcon(":/Icons/Zoom"), tr("&Zoom"));
	zoomAction->setStatusTip(tr("Zoom view in"));
	zoomAction->setShortcut(QKeySequence::ZoomIn);
	connect(zoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::zoom);

	unzoomAction = viewMenu->addAction(QIcon(":/Icons/Unzoom"), tr("&Unzoom"));
	unzoomAction->setStatusTip(tr("Zoom view out"));
	unzoomAction->setShortcut(QKeySequence::ZoomOut);
	connect(unzoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::unzoom);

	resetZoomAction = viewMenu->addAction(QIcon(":/Icons/Zoom-Reset"), tr("&Zoom 1:1"));
	resetZoomAction->setStatusTip(tr("Reset view zoom to 1:1"));
	connect(resetZoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::resetZoom);

	fitZoomAction = viewMenu->addAction(QIcon(":/Icons/Zoom-Fit"), tr("&Fit to View"));
	fitZoomAction->setStatusTip(tr("Zoom to fit all the items to view"));
	connect(fitZoomAction, &QAction::triggered, m_editorView, &CEditorView::fitToView);

	connect(m_editorView, SIGNAL(scaleChanged(double)), this, SLOT(onZoomChanged(double)));


	// add view toolbar
	QToolBar *zoomToolbar = parent->addToolBar(tr("View"));
	zoomToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	zoomToolbar->addAction(zoomAction);

	resetZoomAction2 = zoomToolbar->addAction(QIcon(":/Icons/Zoom-Reset"), "");
	resetZoomAction2->setStatusTip(resetZoomAction->statusTip());
	connect(resetZoomAction2, &QAction::triggered, this, &qvgeNodeEditorUIController::resetZoom);

	zoomToolbar->addAction(unzoomAction);
	zoomToolbar->addAction(fitZoomAction);


	// update actions
	onSelectionChanged();
	onZoomChanged(1);
}

qvgeNodeEditorUIController::~qvgeNodeEditorUIController() 
{
	
}


void qvgeNodeEditorUIController::onSelectionChanged()
{
	int selectionCount = m_scene->selectedItems().size();
	cutAction->setEnabled(selectionCount > 0);
	copyAction->setEnabled(selectionCount > 0);
	delAction->setEnabled(selectionCount > 0);
}


void qvgeNodeEditorUIController::onZoomChanged(double currentZoom)
{
	resetZoomAction2->setText(QString("%1%").arg((int)(currentZoom * 100)));
}


void qvgeNodeEditorUIController::zoom()
{
	m_editorView->zoomBy(1.3);
}


void qvgeNodeEditorUIController::unzoom()
{
	m_editorView->zoomBy(1.0 / 1.3);
}


void qvgeNodeEditorUIController::resetZoom()
{
	m_editorView->zoomTo(1.0);
}

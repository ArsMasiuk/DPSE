#include "qvgeNodeEditorUIController.h"

#include <QMenuBar>


qvgeNodeEditorUIController::qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view) : QObject(parent)
{
	// connect scene
	connect(scene, &CEditorScene::sceneChanged, parent, &CMainWindow::onDocumentChanged);

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

	// add edit toolbar
	QToolBar *editToolbar = parent->addToolBar(tr("Edit"));
	editToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	editToolbar->addAction(undoAction);
	editToolbar->addAction(redoAction);
}

qvgeNodeEditorUIController::~qvgeNodeEditorUIController() 
{
	
}

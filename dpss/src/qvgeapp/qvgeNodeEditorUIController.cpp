#include "qvgeNodeEditorUIController.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>

#include <qvge/CNode.h>
#include <qvge/CImageExport.h>
#include <qvge/CPDFExport.h>

#include <common/CCommutationTable.h>
#include <common/CSceneOptionsDialog.h>
#include <common/CNodePropertiesUI.h>


qvgeNodeEditorUIController::qvgeNodeEditorUIController(CMainWindow *parent, CNodeEditorScene *scene, CEditorView *view) : 
	QObject(parent),
	m_parent(parent), m_scene(scene), m_editorView(view)
{
	// connect scene
	connect(scene, &CEditorScene::sceneChanged, parent, &CMainWindow::onDocumentChanged);
	connect(scene, &CEditorScene::selectionChanged, this, &qvgeNodeEditorUIController::onSelectionChanged);

	// connect view
	connect(m_editorView, SIGNAL(scaleChanged(double)), this, SLOT(onZoomChanged(double)));

	// menus & actions
	createMenus();

	// dock panels
	createPanels();
}


void qvgeNodeEditorUIController::createMenus()
{
	// file actions
	QAction *exportAction = m_parent->getFileExportAction();
	exportAction->setVisible(true);
	exportAction->setText(tr("Export to &Image..."));
	connect(exportAction, &QAction::triggered, this, &qvgeNodeEditorUIController::exportFile);

	QAction *exportActionPDF = new QAction(tr("Export to &PDF..."));
	m_parent->getFileMenu()->insertAction(exportAction, exportActionPDF);
	connect(exportActionPDF, &QAction::triggered, this, &qvgeNodeEditorUIController::exportPDF);

	m_parent->getFileMenu()->insertSeparator(exportActionPDF);


	// add edit menu
	QMenu *editMenu = new QMenu(tr("&Edit"));
	m_parent->menuBar()->insertMenu(m_parent->getWindowMenuAction(), editMenu);

	QAction *undoAction = editMenu->addAction(QIcon(":/Icons/Undo"), tr("&Undo"));
	undoAction->setStatusTip(tr("Undo latest action"));
	undoAction->setShortcut(QKeySequence::Undo);
	connect(undoAction, &QAction::triggered, m_scene, &CEditorScene::undo);
	connect(m_scene, &CEditorScene::undoAvailable, undoAction, &QAction::setEnabled);
	undoAction->setEnabled(m_scene->availableUndoCount());

	QAction *redoAction = editMenu->addAction(QIcon(":/Icons/Redo"), tr("&Redo"));
	redoAction->setStatusTip(tr("Redo latest action"));
	redoAction->setShortcut(QKeySequence::Redo);
	connect(redoAction, &QAction::triggered, m_scene, &CEditorScene::redo);
	connect(m_scene, &CEditorScene::redoAvailable, redoAction, &QAction::setEnabled);
	redoAction->setEnabled(m_scene->availableRedoCount());

	editMenu->addSeparator();

	cutAction = editMenu->addAction(QIcon(":/Icons/Cut"), tr("Cu&t"));
	cutAction->setStatusTip(tr("Cut selection to clipboard"));
	cutAction->setShortcut(QKeySequence::Cut);
	connect(cutAction, &QAction::triggered, m_scene, &CEditorScene::cut);

	copyAction = editMenu->addAction(QIcon(":/Icons/Copy"), tr("&Copy"));
	copyAction->setStatusTip(tr("Copy selection to clipboard"));
	copyAction->setShortcut(QKeySequence::Copy);
	connect(copyAction, &QAction::triggered, m_scene, &CEditorScene::copy);

	pasteAction = editMenu->addAction(QIcon(":/Icons/Paste"), tr("&Paste"));
	pasteAction->setStatusTip(tr("Paste selection from clipboard"));
	pasteAction->setShortcut(QKeySequence::Paste);
	connect(pasteAction, &QAction::triggered, m_scene, &CEditorScene::paste);

	delAction = editMenu->addAction(QIcon(":/Icons/Delete"), tr("&Delete"));
	delAction->setStatusTip(tr("Delete selection"));
	delAction->setShortcut(QKeySequence::Delete);
	connect(delAction, &QAction::triggered, m_scene, &CEditorScene::del);

	editMenu->addSeparator();

	unlinkAction = editMenu->addAction(QIcon(":/Icons/Unlink"), tr("&Unlink"));
	unlinkAction->setStatusTip(tr("Unlink selected nodes"));
	connect(unlinkAction, &QAction::triggered, m_scene, &CNodeEditorScene::onActionUnlink);

	// add edit toolbar
	QToolBar *editToolbar = m_parent->addToolBar(tr("Edit"));
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
	m_parent->menuBar()->insertMenu(m_parent->getWindowMenuAction(), viewMenu);

	QAction *gridAction = viewMenu->addAction(QIcon(":/Icons/Grid-Show"), tr("Show &Grid"));
	gridAction->setCheckable(true);
	gridAction->setStatusTip(tr("Show/hide background grid"));
	gridAction->setChecked(m_scene->gridEnabled());
	connect(gridAction, SIGNAL(toggled(bool)), m_scene, SLOT(enableGrid(bool)));

	QAction *gridSnapAction = viewMenu->addAction(QIcon(":/Icons/Grid-Snap"), tr("&Snap to Grid"));
	gridSnapAction->setCheckable(true);
	gridSnapAction->setStatusTip(tr("Snap to grid when dragging"));
	gridSnapAction->setChecked(m_scene->gridSnapEnabled());
	connect(gridSnapAction, SIGNAL(toggled(bool)), m_scene, SLOT(enableGridSnap(bool)));

	QAction *actionShowLabels = viewMenu->addAction(QIcon(":/Icons/Label"), tr("Show &Labels"));
	actionShowLabels->setCheckable(true);
	actionShowLabels->setStatusTip(tr("Show/hide item labels"));
	actionShowLabels->setChecked(m_scene->itemLabelsEnabled());
	connect(actionShowLabels, SIGNAL(toggled(bool)), m_scene, SLOT(enableItemLabels(bool)));

	viewMenu->addSeparator();

	zoomAction = viewMenu->addAction(QIcon(":/Icons/ZoomIn"), tr("&Zoom"));
	zoomAction->setStatusTip(tr("Zoom view in"));
	zoomAction->setShortcut(QKeySequence::ZoomIn);
	connect(zoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::zoom);

	unzoomAction = viewMenu->addAction(QIcon(":/Icons/ZoomOut"), tr("&Unzoom"));
	unzoomAction->setStatusTip(tr("Zoom view out"));
	unzoomAction->setShortcut(QKeySequence::ZoomOut);
	connect(unzoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::unzoom);

	resetZoomAction = viewMenu->addAction(QIcon(":/Icons/ZoomReset"), tr("&Reset Zoom"));
	resetZoomAction->setStatusTip(tr("Zoom view to 100%"));
	connect(resetZoomAction, &QAction::triggered, this, &qvgeNodeEditorUIController::resetZoom);

	fitZoomAction = viewMenu->addAction(QIcon(":/Icons/ZoomFit"), tr("&Fit to View"));
	fitZoomAction->setStatusTip(tr("Zoom to fit all the items to view"));
	connect(fitZoomAction, &QAction::triggered, m_editorView, &CEditorView::fitToView);


	// add view toolbar
	QToolBar *zoomToolbar = m_parent->addToolBar(tr("View"));
	zoomToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	zoomToolbar->addAction(zoomAction);

	resetZoomAction2 = zoomToolbar->addAction(QIcon(":/Icons/Zoom"), "");
	resetZoomAction2->setStatusTip(resetZoomAction->statusTip());
	resetZoomAction2->setToolTip(resetZoomAction->statusTip());
	connect(resetZoomAction2, &QAction::triggered, this, &qvgeNodeEditorUIController::resetZoom);

	zoomToolbar->addAction(unzoomAction);
	zoomToolbar->addAction(fitZoomAction);


	// update actions
	onSelectionChanged();
	onZoomChanged(1);
}


void qvgeNodeEditorUIController::createPanels()
{
	// propertis
	QDockWidget *propertyDock = new QDockWidget(tr("Properties"));
	m_parent->addDockWidget(Qt::RightDockWidgetArea, propertyDock);

    CNodePropertiesUI *nodesUI = new CNodePropertiesUI(propertyDock);

    propertyDock->setWidget(nodesUI);

	// connections
	QDockWidget *connectionsDock = new QDockWidget(tr("Connections"));
	m_parent->addDockWidget(Qt::RightDockWidgetArea, connectionsDock);
	CCommutationTable *connectionsPanel = new CCommutationTable(connectionsDock);
	connectionsDock->setWidget(connectionsPanel);
	connectionsPanel->setScene(m_scene);
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

	auto nodes = m_scene->getSelectedItems<CNode>();
	unlinkAction->setEnabled(nodes.size() > 0);
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


void qvgeNodeEditorUIController::exportFile()
{
	if (CImageExport::write(*m_scene, m_parent->getCurrentFileName()))
	{
		m_parent->statusBar()->showMessage(tr("Export successful"));
	}
	else
	{
		m_parent->statusBar()->showMessage(tr("Export failed"));
	}
}


void qvgeNodeEditorUIController::exportPDF()
{
	if (CPDFExport::write(*m_scene, m_parent->getCurrentFileName()))
	{
		m_parent->statusBar()->showMessage(tr("Export successful"));
	}
	else
	{
		m_parent->statusBar()->showMessage(tr("Export failed"));
	}
}

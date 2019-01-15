﻿/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <CNodeEditorUIController.h>
#include <CColorSchemesUIController.h>
#include <CSceneMenuUIController.h>
#include <CCommutationTable.h>
#include <CNodeEdgePropertiesUI.h>
#include <CClassAttributesEditorUI.h>
#include <CExtListInputDialog.h>
#include <CNodesFactorDialog.h>
#include <CNodePortEditorDialog.h>
#include <CSearchDialog.h>

#ifdef USE_OGDF
#include <ogdf/COGDFLayoutUIController.h>
#include <ogdf/COGDFNewGraphDialog.h>
#include <ogdf/COGDFLayout.h>
#endif

#include <appbase/CMainWindow.h>

#include <qvge/CNode.h>
#include <qvge/CEdge.h>
#include <qvge/CImageExport.h>
#include <qvge/CPDFExport.h>
#include <qvge/CNodeEditorScene.h>
#include <qvge/CNodeSceneActions.h>
#include <qvge/CEditorSceneDefines.h>
#include <qvge/CEditorView.h>
#include <qvge/CFileSerializerGEXF.h>
#include <qvge/CFileSerializerGraphML.h>
#include <qvge/CFileSerializerXGR.h>
#include <qvge/CFileSerializerDOT.h>
#include <qvge/CFileSerializerCSV.h>
#include <qvge/ISceneItemFactory.h>

#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QMenu>
#include <QToolButton>
#include <QWidgetAction>
#include <QResizeEvent>
#include <QDebug>
#include <QPixmapCache>
#include <QFileDialog>
#include <QTimer>


CNodeEditorUIController::CNodeEditorUIController(CMainWindow *parent) :
    QObject(parent),
    m_parent(parent)
{
	// backup timer
	connect(&m_backupTimer, &QTimer::timeout, this, &CNodeEditorUIController::doBackup);

    // create document
    m_editorScene = new CNodeEditorScene(parent);
    m_editorView = new CEditorView(m_editorScene, parent);
    parent->setCentralWidget(m_editorView);

    // connect scene
    connect(m_editorScene, &CEditorScene::sceneChanged, parent, &CMainWindow::onDocumentChanged);
    connect(m_editorScene, &CEditorScene::sceneChanged, this, &CNodeEditorUIController::onSceneChanged);
    connect(m_editorScene, &CEditorScene::selectionChanged, this, &CNodeEditorUIController::onSelectionChanged);

    connect(m_editorScene, &CEditorScene::infoStatusChanged, this, &CNodeEditorUIController::onSceneStatusChanged);
    connect(m_editorScene, &CNodeEditorScene::editModeChanged, this, &CNodeEditorUIController::onEditModeChanged);

	connect(m_editorScene, &CEditorScene::sceneDoubleClicked, this, &CNodeEditorUIController::onSceneDoubleClicked);

    CSceneMenuUIController *menuController = new CSceneMenuUIController(this);
    m_editorScene->setContextMenuController(menuController);

    // connect view
    connect(m_editorView, SIGNAL(scaleChanged(double)), this, SLOT(onZoomChanged(double)));

    // slider2d
    createNavigator();

    // menus & actions
    createMenus();

    // dock panels
    createPanels();

    // status bar
    m_statusLabel = new QLabel();
    parent->statusBar()->addPermanentWidget(m_statusLabel);

    // update actions
    onSceneChanged();
    onSelectionChanged();
    onZoomChanged(1);
    onSceneStatusChanged(m_editorScene->getInfoStatus());

    // search dialog
    m_searchDialog = new CSearchDialog(parent);

    // OGDF
#ifdef USE_OGDF
    m_ogdfController = new COGDFLayoutUIController(parent, m_editorScene);
#endif

    // workaround for full screen
#ifndef Q_OS_WIN32
    if (parent->isMaximized())
    {
        parent->showNormal();
        QTimer::singleShot(0, parent, SLOT(showMaximized()));
    }
#endif

	// default scene settings
	readDefaultSceneSettings();
}


CNodeEditorUIController::~CNodeEditorUIController()
{
}


void CNodeEditorUIController::createMenus()
{
    // file actions
    QAction *exportAction = m_parent->getFileExportAction();
    exportAction->setVisible(true);
    exportAction->setText(tr("Export to &Image..."));
    connect(exportAction, &QAction::triggered, this, &CNodeEditorUIController::exportFile);

    QAction *exportActionPDF = new QAction(tr("Export to &PDF..."));
    m_parent->getFileMenu()->insertAction(exportAction, exportActionPDF);
    connect(exportActionPDF, &QAction::triggered, this, &CNodeEditorUIController::exportPDF);

    QAction *exportActionDOT = new QAction(tr("Export to &DOT/GraphViz..."));
    m_parent->getFileMenu()->insertAction(exportActionPDF, exportActionDOT);
    connect(exportActionDOT, &QAction::triggered, this, &CNodeEditorUIController::exportDOT);

    m_parent->getFileMenu()->insertSeparator(exportActionDOT);


    // add edit menu
    QMenu *editMenu = new QMenu(tr("&Edit"));
    m_parent->menuBar()->insertMenu(m_parent->getWindowMenuAction(), editMenu);

    QAction *undoAction = editMenu->addAction(QIcon(":/Icons/Undo"), tr("&Undo"));
    undoAction->setStatusTip(tr("Undo latest action"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, this, &CNodeEditorUIController::undo);
    connect(m_editorScene, &CEditorScene::undoAvailable, undoAction, &QAction::setEnabled);
    undoAction->setEnabled(m_editorScene->availableUndoCount());

    QAction *redoAction = editMenu->addAction(QIcon(":/Icons/Redo"), tr("&Redo"));
    redoAction->setStatusTip(tr("Redo latest action"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, this, &CNodeEditorUIController::redo);
    connect(m_editorScene, &CEditorScene::redoAvailable, redoAction, &QAction::setEnabled);
    redoAction->setEnabled(m_editorScene->availableRedoCount());

    editMenu->addSeparator();

    cutAction = editMenu->addAction(QIcon(":/Icons/Cut"), tr("Cu&t"));
    cutAction->setStatusTip(tr("Cut selected item(s) to clipboard"));
    cutAction->setToolTip(tr("Cut selection"));
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, m_editorScene, &CEditorScene::cut);

    copyAction = editMenu->addAction(QIcon(":/Icons/Copy"), tr("&Copy"));
    copyAction->setStatusTip(tr("Copy selected item(s) to clipboard"));
    copyAction->setToolTip(tr("Copy selection"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, m_editorScene, &CEditorScene::copy);

    pasteAction = editMenu->addAction(QIcon(":/Icons/Paste"), tr("&Paste"));
    pasteAction->setStatusTip(tr("Paste item(s) from clipboard to the area center"));
    pasteAction->setToolTip(tr("Paste from clipboard"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, this, &CNodeEditorUIController::paste);

	pasteInplaceAction = editMenu->addAction(tr("Paste In Place"));
	pasteInplaceAction->setStatusTip(tr("Paste item(s) from clipboard with the same coordinates"));
	pasteInplaceAction->setToolTip(tr("Paste from clipboard in-place"));
	connect(pasteInplaceAction, SIGNAL(triggered()), m_editorScene, SLOT(paste()));

    delAction = editMenu->addAction(QIcon(":/Icons/Delete"), tr("&Delete"));
    delAction->setStatusTip(tr("Delete selected item(s)"));
    delAction->setToolTip(tr("Delete selection"));
    delAction->setShortcut(QKeySequence::Delete);
    connect(delAction, &QAction::triggered, m_editorScene, &CEditorScene::del);

	QAction *selAction = editMenu->addAction(QIcon(":/Icons/SelectAll"), tr("Select All"));
	selAction->setStatusTip(tr("Select all items on the scene"));
	selAction->setToolTip(tr("Select all items"));
	selAction->setShortcut(QKeySequence::SelectAll);
	connect(selAction, &QAction::triggered, m_editorScene, &CEditorScene::selectAll);


    // edit modes
    editMenu->addSeparator();

    m_editModesGroup = new QActionGroup(this);
    m_editModesGroup->setExclusive(true);
    connect(m_editModesGroup, &QActionGroup::triggered, this, &CNodeEditorUIController::sceneEditMode);

    modeDefaultAction = editMenu->addAction(QIcon(":/Icons/Mode-Select"), tr("Select Items"));
    modeDefaultAction->setToolTip(tr("Items selection mode"));
    modeDefaultAction->setStatusTip(tr("Select/deselect items in the document"));
    modeDefaultAction->setCheckable(true);
    modeDefaultAction->setActionGroup(m_editModesGroup);
    modeDefaultAction->setChecked(m_editorScene->getEditMode() == EM_Default);
    modeDefaultAction->setData(EM_Default);

    modeNodesAction = editMenu->addAction(QIcon(":/Icons/Mode-AddNodes"), tr("Create Nodes"));
    modeNodesAction->setToolTip(tr("Adding new nodes mode"));
    modeNodesAction->setStatusTip(tr("Quickly add nodes & edges"));
    modeNodesAction->setCheckable(true);
    modeNodesAction->setActionGroup(m_editModesGroup);
    modeNodesAction->setChecked(m_editorScene->getEditMode() == EM_AddNodes);
    modeNodesAction->setData(EM_AddNodes);

    //modeEdgesAction = editMenu->addAction(tr("Add edges mode"));
    //modeEdgesAction->setCheckable(true);
    //modeEdgesAction->setActionGroup(m_editModesGroup);
    //modeEdgesAction->setChecked(m_editorScene->getEditMode() == EM_AddEdges);
    //modeEdgesAction->setData(EM_AddEdges);


    // scene actions
    editMenu->addSeparator();

    QAction *sceneCropAction = editMenu->addAction(QIcon(":/Icons/Crop"), tr("&Crop Area"));
    sceneCropAction->setStatusTip(tr("Crop document area to contents"));
    connect(sceneCropAction, &QAction::triggered, m_editorScene, &CEditorScene::crop);


    // color schemes
    editMenu->addSeparator();

    m_schemesController = new CColorSchemesUIController(this);
    m_schemesController->setScene(m_editorScene);
    QAction *schemesAction = editMenu->addMenu(m_schemesController->getSchemesMenu());
    schemesAction->setText(tr("Apply Colors"));
    schemesAction->setStatusTip(tr("Apply predefined color scheme to the document"));


    // scene options
    editMenu->addSeparator();

    QAction *sceneAction = editMenu->addAction(QIcon(":/Icons/Settings"), tr("&Options..."));
    sceneAction->setStatusTip(tr("Change document properties"));
    connect(sceneAction, &QAction::triggered, this, &CNodeEditorUIController::sceneOptions);


    // add edit toolbar
    QToolBar *editToolbar = m_parent->addToolBar(tr("Edit"));
    editToolbar->setObjectName("editToolbar");
    editToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    editToolbar->addAction(undoAction);
    editToolbar->addAction(redoAction);

    editToolbar->addSeparator();

    editToolbar->addAction(cutAction);
    editToolbar->addAction(copyAction);
    editToolbar->addAction(pasteAction);
    editToolbar->addAction(delAction);

    editToolbar->addSeparator();

    // add edit modes toolbar
    QToolBar *editModesToolbar = m_parent->addToolBar(tr("Edit Modes"));
    editModesToolbar->setObjectName("editModesToolbar");
    editModesToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    editModesToolbar->addAction(modeDefaultAction);
    editModesToolbar->addAction(modeNodesAction);


    // add view menu
    m_viewMenu = new QMenu(tr("&View"));
    m_parent->menuBar()->insertMenu(m_parent->getWindowMenuAction(), m_viewMenu);

    gridAction = m_viewMenu->addAction(QIcon(":/Icons/Grid-Show"), tr("Show &Grid"));
    gridAction->setCheckable(true);
    gridAction->setStatusTip(tr("Show/hide background grid"));
    gridAction->setChecked(m_editorScene->gridEnabled());
    connect(gridAction, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGrid(bool)));

    gridSnapAction = m_viewMenu->addAction(QIcon(":/Icons/Grid-Snap"), tr("&Snap to Grid"));
    gridSnapAction->setCheckable(true);
    gridSnapAction->setStatusTip(tr("Snap to grid when dragging"));
    gridSnapAction->setChecked(m_editorScene->gridSnapEnabled());
    connect(gridSnapAction, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGridSnap(bool)));

 //   actionShowLabels = m_viewMenu->addAction(QIcon(":/Icons/Label"), tr("Show &Labels"));
 //   actionShowLabels->setCheckable(true);
 //   actionShowLabels->setStatusTip(tr("Show/hide item labels"));
	//actionShowLabels->setChecked(m_editorScene->isClassAttributeVisible("item", "label"));
	//connect(actionShowLabels, SIGNAL(toggled(bool)), this, SLOT(showItemLabels(bool)));

	m_actionShowNodeIds = m_viewMenu->addAction(tr("Show Node Ids"));
	m_actionShowNodeIds->setCheckable(true);
	m_actionShowNodeIds->setStatusTip(tr("Show/hide node ids"));
	m_actionShowNodeIds->setChecked(m_editorScene->isClassAttributeVisible(class_node, attr_id));
	connect(m_actionShowNodeIds, SIGNAL(toggled(bool)), this, SLOT(showNodeIds(bool)));

	m_actionShowEdgeIds = m_viewMenu->addAction(tr("Show Edge Ids"));
	m_actionShowEdgeIds->setCheckable(true);
	m_actionShowEdgeIds->setStatusTip(tr("Show/hide edge ids"));
	m_actionShowEdgeIds->setChecked(m_editorScene->isClassAttributeVisible(class_edge, attr_id));
	connect(m_actionShowEdgeIds, SIGNAL(toggled(bool)), this, SLOT(showEdgeIds(bool)));

	m_viewMenu->addSeparator();

	findAction = m_viewMenu->addAction(QIcon(":/Icons/Search"), tr("&Find..."));
	findAction->setStatusTip(tr("Search for items and attributes"));
	//findAction->setToolTip(tr("Find text"));
	findAction->setShortcut(QKeySequence::Find);
	connect(findAction, &QAction::triggered, this, &CNodeEditorUIController::find);

	m_viewMenu->addSeparator();

    zoomAction = m_viewMenu->addAction(QIcon(":/Icons/ZoomIn"), tr("&Zoom"));
    zoomAction->setStatusTip(tr("Zoom view in"));
    zoomAction->setShortcut(QKeySequence::ZoomIn);
    connect(zoomAction, &QAction::triggered, this, &CNodeEditorUIController::zoom);

    unzoomAction = m_viewMenu->addAction(QIcon(":/Icons/ZoomOut"), tr("&Unzoom"));
    unzoomAction->setStatusTip(tr("Zoom view out"));
    unzoomAction->setShortcut(QKeySequence::ZoomOut);
    connect(unzoomAction, &QAction::triggered, this, &CNodeEditorUIController::unzoom);

    resetZoomAction = m_viewMenu->addAction(QIcon(":/Icons/ZoomReset"), tr("&Reset Zoom"));
    resetZoomAction->setStatusTip(tr("Zoom view to 100%"));
    connect(resetZoomAction, &QAction::triggered, this, &CNodeEditorUIController::resetZoom);

    fitZoomAction = m_viewMenu->addAction(QIcon(":/Icons/ZoomFit"), tr("&Fit to View"));
    fitZoomAction->setStatusTip(tr("Zoom to fit all the items to view"));
    connect(fitZoomAction, &QAction::triggered, m_editorView, &CEditorView::fitToView);

    fitZoomSelectedAction = m_viewMenu->addAction(QIcon(":/Icons/ZoomFitSelected"), tr("Fit &Selection to View"));
    fitZoomSelectedAction->setStatusTip(tr("Zoom to fit selected items to view"));
    connect(fitZoomSelectedAction, &QAction::triggered, m_editorView, &CEditorView::fitSelectedToView);


    // add view toolbar
    QToolBar *zoomToolbar = m_parent->addToolBar(tr("View"));
    zoomToolbar->setObjectName("viewToolbar");
    zoomToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	zoomToolbar->addAction(findAction);

	zoomToolbar->addSeparator();

    zoomToolbar->addAction(zoomAction);

    resetZoomAction2 = zoomToolbar->addAction(QIcon(":/Icons/Zoom"), "");
    resetZoomAction2->setStatusTip(resetZoomAction->statusTip());
    resetZoomAction2->setToolTip(resetZoomAction->statusTip());
    connect(resetZoomAction2, &QAction::triggered, this, &CNodeEditorUIController::resetZoom);

    zoomToolbar->addAction(unzoomAction);
    zoomToolbar->addAction(fitZoomAction);
    zoomToolbar->addAction(fitZoomSelectedAction);
}


void CNodeEditorUIController::createPanels()
{
    // properties
    m_parent->createDockWindow(
        "propertyDock", tr("Item Properties"), Qt::RightDockWidgetArea,
        m_propertiesPanel = new CNodeEdgePropertiesUI(m_parent)
    );

    m_propertiesPanel->setScene(m_editorScene);


    // connections
    m_parent->createDockWindow(
        "connectionsDock", tr("Topology"), Qt::RightDockWidgetArea,
        m_connectionsPanel = new CCommutationTable(m_parent)
    );

    m_connectionsPanel->setScene(m_editorScene);


    // default properties
    m_parent->createDockWindow(
        "defaultsDock", tr("Default Properties"), Qt::LeftDockWidgetArea,
        m_defaultsPanel = new CClassAttributesEditorUI(m_parent)
    );

    m_defaultsPanel->setScene(m_editorScene);


	// update view menu with created toolbars & panels
	m_viewMenu->addSeparator();
	QAction *panelsAction = m_viewMenu->addMenu(m_parent->createPopupMenu());
	panelsAction->setText("Toolbars and Panels");
}


void CNodeEditorUIController::createNavigator()
{
    m_sliderView = new QSint::Slider2d(m_parent);
    m_sliderView->connectSource(m_editorView);

    QToolButton *sliderButton = m_sliderView->makeAsButton();
    m_editorView->setCornerWidget(sliderButton);

    sliderButton->setIcon(QIcon(":/Icons/Navigator"));
    sliderButton->setToolTip(tr("Show scene navigator"));
    connect(m_sliderView, SIGNAL(aboutToShow()), this, SLOT(onNavigatorShown()));

    m_sliderView->setFixedSize(200,200);
    m_sliderView->setSliderOpacity(0.3);
    m_sliderView->setSliderBrush(Qt::green);
}


void CNodeEditorUIController::onNavigatorShown()
{
    double w = m_editorScene->sceneRect().width();
    double h = m_editorScene->sceneRect().height();
    double cw = w > h ? 200.0 : 200.0 * (w/h);
    double ch = h > w ? 200.0 : 200.0 * (h/w) ;
    m_sliderView->setFixedSize(cw, ch);

    // Qt bug: update menu size
    QResizeEvent re(m_sliderView->size(), m_sliderView->parentWidget()->size());
    qApp->sendEvent(m_sliderView->parentWidget(), &re);

    QPixmap pm(m_sliderView->size());
    QPainter p(&pm);
    bool gridOn = m_editorScene->gridEnabled();
    bool labelsOn = m_editorScene->itemLabelsEnabled();
    m_editorScene->enableGrid(false);
    m_editorScene->enableItemLabels(false);
    m_editorScene->render(&p);
    m_editorScene->enableGrid(gridOn);
    m_editorScene->enableItemLabels(labelsOn);
    m_sliderView->setBackgroundBrush(pm);
}


void CNodeEditorUIController::onSelectionChanged()
{
    int selectionCount = m_editorScene->selectedItems().size();

    cutAction->setEnabled(selectionCount > 0);
    copyAction->setEnabled(selectionCount > 0);
    delAction->setEnabled(selectionCount > 0);

    fitZoomSelectedAction->setEnabled(selectionCount > 0);
}


void CNodeEditorUIController::onSceneChanged()
{
    auto nodes = m_editorScene->getItems<CNode>();
    auto edges = m_editorScene->getItems<CEdge>();

    m_statusLabel->setText(tr("Nodes: %1 | Edges: %2").arg(nodes.size()).arg(edges.size()));

	updateActions();
}


void CNodeEditorUIController::onSceneHint(const QString& text)
{
    m_parent->statusBar()->showMessage(text);
}


void CNodeEditorUIController::onSceneStatusChanged(int status)
{
	bool isAddNodesMode = (m_editorScene->getEditMode() == EM_AddNodes);
 
    switch (status)
    {
    case SIS_Hover:
		if (isAddNodesMode)
			onSceneHint(tr("Click & drag - create new connection"));
		else
			onSceneHint(tr("Ctrl+Click - (un)select item | Click & drag - move selected items | Ctrl+Click & drag - clone selected items"));
        return;

    case SIS_Drag:
        onSceneHint(tr("Shift - horizontal or vertical snap | Alt - toggle grid snap"));
        return;

    case SIS_Hover_Port:
        onSceneHint(tr("Click & drag - make a connection at this port"));
        return;

    default:
		if (isAddNodesMode)
			onSceneHint(tr("Click - create new node | Click & drag - create new connection"));
		else
			onSceneHint(tr("Click & drag - select an area"));
    }
}


void CNodeEditorUIController::onSceneDoubleClicked(QGraphicsSceneMouseEvent* /*mouseEvent*/, QGraphicsItem* clickedItem)
{
	CNodePort *port = dynamic_cast<CNodePort*>(clickedItem);
	if (port)
		editNodePort(*port);
}


void CNodeEditorUIController::onZoomChanged(double currentZoom)
{
    resetZoomAction2->setText(QString("%1%").arg((int)(currentZoom * 100)));
}


void CNodeEditorUIController::zoom()
{
    m_editorView->zoomBy(1.3);
}


void CNodeEditorUIController::unzoom()
{
    m_editorView->zoomBy(1.0 / 1.3);
}


void CNodeEditorUIController::resetZoom()
{
    m_editorView->zoomTo(1.0);
}


void CNodeEditorUIController::sceneOptions()
{
    CSceneOptionsDialog dialog;

    if (dialog.exec(*m_editorScene, *m_editorView, m_optionsData))
    {
		updateSceneOptions();

        m_parent->writeSettings();
    }
}


void CNodeEditorUIController::updateSceneOptions()
{
	if (m_optionsData.backupPeriod > 0) {
		m_backupTimer.setInterval(m_optionsData.backupPeriod * 60000);
		m_backupTimer.start();
	}
	else
		m_backupTimer.stop();

	updateActions();
}


void CNodeEditorUIController::updateActions()
{
	if (m_editorScene)
	{
		gridAction->setChecked(m_editorScene->gridEnabled());
		gridSnapAction->setChecked(m_editorScene->gridSnapEnabled());
		m_actionShowNodeIds->setChecked(m_editorScene->isClassAttributeVisible(class_node, attr_id));
		m_actionShowEdgeIds->setChecked(m_editorScene->isClassAttributeVisible(class_edge, attr_id));
	}
}


void CNodeEditorUIController::updateFromActions()
{
	if (m_editorScene)
	{
		m_editorScene->setClassAttributeVisible(class_node, attr_id, m_actionShowNodeIds->isChecked());
		m_editorScene->setClassAttributeVisible(class_edge, attr_id, m_actionShowEdgeIds->isChecked());
	}
}


void CNodeEditorUIController::sceneEditMode(QAction* act)
{
    int mode = act->data().toInt();
    m_editorScene->setEditMode((EditMode)mode);
}


void CNodeEditorUIController::onEditModeChanged(int mode)
{
    if (mode == EM_AddNodes)
        modeNodesAction->setChecked(true);
    else
        modeDefaultAction->setChecked(true);
}


bool CNodeEditorUIController::doExport(const IFileSerializer &exporter)
{
    QString fileName = CUtils::cutLastSuffix(m_parent->getCurrentFileName());
    if (fileName.isEmpty())
        fileName = m_lastExportPath;
    else
        fileName = QFileInfo(m_lastExportPath).absolutePath() + "/" + QFileInfo(fileName).fileName();

    QString path = QFileDialog::getSaveFileName(nullptr,
        QObject::tr("Export as") + " " + exporter.description(),
        fileName,
        exporter.filters()
    );

    if (path.isEmpty())
        return false;

    m_lastExportPath = path;

    if (exporter.save(path, *m_editorScene))
    {
        m_parent->statusBar()->showMessage(tr("Export successful (%1)").arg(path));
        return true;
    }
    else
    {
        m_parent->statusBar()->showMessage(tr("Export failed (%1)").arg(path));
        return false;
    }
}


void CNodeEditorUIController::exportFile()
{
    doExport(CImageExport());
}


void CNodeEditorUIController::exportDOT()
{
    doExport(CFileSerializerDOT());
}


void CNodeEditorUIController::exportPDF()
{
    doExport(CPDFExport());
}


void CNodeEditorUIController::doBackup()
{
	QString backupFileName = m_parent->getCurrentFileName();
	if (backupFileName.isEmpty()) {
		m_parent->statusBar()->showMessage(tr("Cannot backup non-saved document"), 2000);
		return;
	}
	else {
		backupFileName = CUtils::cutLastSuffix(backupFileName) + ".bak.xgr";
	}

	m_parent->statusBar()->showMessage(tr("Running backup... (%1)").arg(backupFileName));
	qApp->processEvents();

	CFileSerializerXGR writer;
	if (writer.save(backupFileName, *m_editorScene)) {
		m_parent->statusBar()->showMessage(tr("Backup done (%1)").arg(backupFileName), 2000);
	}
	else {
		m_parent->statusBar()->showMessage(tr("Backup failed (%1)").arg(backupFileName), 2000);
	}
}


void CNodeEditorUIController::doReadSettings(QSettings& settings)
{
	// options
    bool isAA = m_editorView->renderHints().testFlag(QPainter::Antialiasing);
    isAA = settings.value("antialiasing", isAA).toBool();
    m_editorView->setRenderHint(QPainter::Antialiasing, isAA);
    m_editorScene->setFontAntialiased(isAA);

    int cacheRam = QPixmapCache::cacheLimit();
    cacheRam = settings.value("cacheRam", cacheRam).toInt();
    QPixmapCache::setCacheLimit(cacheRam);

    m_lastExportPath = settings.value("lastExportPath", m_lastExportPath).toString();
    
	m_optionsData.newGraphDialogOnStart = settings.value("autoCreateGraphDialog", m_optionsData.newGraphDialogOnStart).toBool();
	m_optionsData.backupPeriod = settings.value("backupPeriod", m_optionsData.backupPeriod).toInt();

	updateSceneOptions();


    // UI elements
    settings.beginGroup("UI/ItemProperties");
    m_propertiesPanel->doReadSettings(settings);
    settings.endGroup();

    settings.beginGroup("UI/ClassAttributes");
    m_defaultsPanel->doReadSettings(settings);
    settings.endGroup();
}


void CNodeEditorUIController::doWriteSettings(QSettings& settings)
{
	// temp
	writeDefaultSceneSettings();


    bool isAA = m_editorView->renderHints().testFlag(QPainter::Antialiasing);
    settings.setValue("antialiasing", isAA);

    int cacheRam = QPixmapCache::cacheLimit();
    settings.setValue("cacheRam", cacheRam);

    settings.setValue("lastExportPath", m_lastExportPath);

    settings.setValue("autoCreateGraphDialog", m_optionsData.newGraphDialogOnStart);
	settings.setValue("backupPeriod", m_optionsData.backupPeriod);


    // UI elements
    settings.beginGroup("UI/ItemProperties");
    m_propertiesPanel->doWriteSettings(settings);
    settings.endGroup();

    settings.beginGroup("UI/ClassAttributes");
    m_defaultsPanel->doWriteSettings(settings);
    settings.endGroup();


    // custom topology of the current document
    settings.beginGroup("CustomFiles");

    QString filename = QFileInfo(m_parent->getCurrentFileName()).fileName();
    if (!filename.isEmpty())
    {
        settings.beginGroup(filename);

        settings.beginGroup("UI/Topology");
        m_connectionsPanel->doWriteSettings(settings);
        settings.endGroup();

        settings.endGroup();
    }

    settings.endGroup();
}


bool CNodeEditorUIController::loadFromFile(const QString &fileName, const QString &format, QString* lastError)
{
    if (format == "xgr")
    {
        return (CFileSerializerXGR().load(fileName, *m_editorScene, lastError));
    }

    if (format == "graphml")
    {
        return (CFileSerializerGraphML().load(fileName, *m_editorScene, lastError));
    }

    if (format == "gexf")
    {
        return (CFileSerializerGEXF().load(fileName, *m_editorScene, lastError));
    }

    if (format == "csv")
    {
        QStringList csvList;
        csvList << ";" << "," << "Tab";

        int index = CExtListInputDialog::getItemIndex(
                    tr("Separator"),
                    tr("Choose a separator of columns:"),
                    csvList);
        if (index < 0)
            return false;

        CFileSerializerCSV csvLoader;
        switch (index)
        {
            case 0:     csvLoader.setDelimiter(';');    break;
            case 1:     csvLoader.setDelimiter(',');    break;
            default:    csvLoader.setDelimiter('\t');   break;
        }

        return (csvLoader.load(fileName, *m_editorScene, lastError));
    }

    // else via ogdf
#ifdef USE_OGDF
    return (COGDFLayout::loadGraph(fileName, *m_editorScene, lastError));
#else
    return false;
#endif
}


bool CNodeEditorUIController::saveToFile(const QString &fileName, const QString &format, QString* lastError)
{
    if (format == "xgr")
        return (CFileSerializerXGR().save(fileName, *m_editorScene, lastError));

    if (format == "dot" || format == "gv")
        return (CFileSerializerDOT().save(fileName, *m_editorScene, lastError));

    if (format == "gexf")
        return (CFileSerializerGEXF().save(fileName, *m_editorScene, lastError));

	if (format == "graphml")
		return (CFileSerializerGraphML().save(fileName, *m_editorScene, lastError));

    return false;
}


void CNodeEditorUIController::readDefaultSceneSettings()
{
	QSettings& settings = m_parent->getApplicationSettings();

	settings.beginGroup("Scene/Defaults");

	bool showNodeIds = settings.value("showNodeIds", true).toBool();
	bool showEdgeIds = settings.value("showEdgeIds", true).toBool();

	QColor bgColor = settings.value("background", m_editorScene->backgroundBrush().color()).value<QColor>();
	QPen gridPen = settings.value("grid.color", m_editorScene->getGridPen()).value<QPen>();
	int gridSize = settings.value("grid.size", m_editorScene->getGridSize()).toInt();
	bool gridEnabled = settings.value("grid.enabled", m_editorScene->gridEnabled()).toBool();
	bool gridSnap = settings.value("grid.snap", m_editorScene->gridSnapEnabled()).toBool();

	settings.endGroup();

	// workaround: always make the labels visible
	m_editorScene->setClassAttributeVisible(class_item, attr_label, true);
	m_editorScene->setClassAttributeVisible(class_node, attr_label, true);
	m_editorScene->setClassAttributeVisible(class_edge, attr_label, true);

	m_editorScene->setClassAttributeVisible(class_node, attr_id, showNodeIds);
	m_editorScene->setClassAttributeVisible(class_edge, attr_id, showEdgeIds);
	m_editorScene->setBackgroundBrush(bgColor);
	m_editorScene->setGridPen(gridPen);
	m_editorScene->setGridSize(gridSize);
	m_editorScene->enableGrid(gridEnabled);
	m_editorScene->enableGridSnap(gridSnap);

	updateFromActions();
}


void CNodeEditorUIController::writeDefaultSceneSettings()
{
	QSettings& settings = m_parent->getApplicationSettings();

	settings.beginGroup("Scene/Defaults");

	bool showNodeIds = m_editorScene->isClassAttributeVisible(class_node, attr_id);
	bool showEdgeIds = m_editorScene->isClassAttributeVisible(class_edge, attr_id);
	//bool showLabels = m_editorScene->isClassAttributeVisible("item", "label");

	settings.setValue("showNodeIds", showNodeIds);
	settings.setValue("showEdgeIds", showEdgeIds);

	settings.setValue("background", m_editorScene->backgroundBrush().color());
	settings.setValue("grid.color", m_editorScene->getGridPen());
	settings.setValue("grid.size", m_editorScene->getGridSize());
	settings.setValue("grid.enabled", m_editorScene->gridEnabled());
	settings.setValue("grid.snap", m_editorScene->gridSnapEnabled());

	settings.endGroup();

	settings.sync();
}


void CNodeEditorUIController::onNewDocumentCreated()
{
	readDefaultSceneSettings();

    m_editorScene->setClassAttribute("", "comment", QString());
    m_editorScene->setClassAttribute("", "creator", QApplication::applicationName() + " " + QApplication::applicationVersion());

#ifdef USE_OGDF
    if (m_optionsData.newGraphDialogOnStart)
    {
        COGDFNewGraphDialog dialog;
        dialog.exec(*m_editorScene);

        bool show = dialog.isShowOnStart();
        if (show != m_optionsData.newGraphDialogOnStart)
        {
			m_optionsData.newGraphDialogOnStart = show;
            m_parent->writeSettings();
        }
    }
#endif

    // store newly created state
    m_editorScene->addUndoState();
}


void CNodeEditorUIController::onDocumentLoaded(const QString &fileName)
{
	QSettings& settings = m_parent->getApplicationSettings();

	// read custom topology of the current document
	settings.beginGroup("CustomFiles");

	QString filename = QFileInfo(fileName).fileName();
	if (!filename.isEmpty() && settings.childGroups().contains(filename))
	{
		settings.beginGroup(filename);

		settings.beginGroup("UI/Topology");
		m_connectionsPanel->doReadSettings(settings);
		settings.endGroup();

		settings.endGroup();
	}

	settings.endGroup();

	// workaround: always make the labels visible
	m_editorScene->setClassAttributeVisible(class_item, attr_label, true);
	m_editorScene->setClassAttributeVisible(class_node, attr_label, true);
	m_editorScene->setClassAttributeVisible(class_edge, attr_label, true);

	// store newly created state
	m_editorScene->setInitialState();
}


// actions

void CNodeEditorUIController::factorNodes()
{
    CNodesFactorDialog dialog;
    if (dialog.exec(*m_editorScene) == QDialog::Accepted)
        m_editorScene->addUndoState();
    else
        m_editorScene->revertUndoState();
}


void CNodeEditorUIController::addNodePort()
{
    CNode *node = dynamic_cast<CNode*>(m_editorScene->getContextMenuTrigger());
    if (!node)
        return;

    CNodePort *port = node->addPort();
    if (!port)
        return;

    CNodePortEditorDialog dialog;
    if (dialog.exec(*port) == QDialog::Accepted)
        m_editorScene->addUndoState();
    else
        delete port;
}


void CNodeEditorUIController::editNodePort()
{
    CNodePort *port = dynamic_cast<CNodePort*>(m_editorScene->getContextMenuTrigger());
	if (port)
		editNodePort(*port);
}


void CNodeEditorUIController::editNodePort(CNodePort &port)
{
	CNodePortEditorDialog dialog;
	if (dialog.exec(port) == QDialog::Accepted)
		m_editorScene->addUndoState();
	else
		m_editorScene->revertUndoState();
}


void CNodeEditorUIController::find()
{
    m_searchDialog->exec(*m_editorScene);
}


void CNodeEditorUIController::paste()
{
	QRectF vp = m_editorView->mapToScene(m_editorView->viewport()->geometry()).boundingRect();
	auto center = vp.center();
	m_editorScene->paste(center);
}


void CNodeEditorUIController::showNodeIds(bool on)
{
	m_editorScene->setClassAttributeVisible(class_node, attr_id, on);

	m_editorScene->addUndoState();
}


void CNodeEditorUIController::showEdgeIds(bool on)
{
	m_editorScene->setClassAttributeVisible(class_edge, attr_id, on);

	m_editorScene->addUndoState();
}


void CNodeEditorUIController::showItemLabels(bool on)
{
	m_editorScene->setClassAttributeVisible(class_item, "label", on);

	m_editorScene->addUndoState();
}


void CNodeEditorUIController::undo()
{
	m_editorScene->undo();

	updateFromActions();
}


void CNodeEditorUIController::redo()
{
	m_editorScene->redo();

	updateFromActions();
}


void CNodeEditorUIController::changeItemId()
{
	auto sceneActions = dynamic_cast<CNodeSceneActions*>(m_editorScene->getActions());
	int nodesCount = m_editorScene->getSelectedNodes().size();
	int edgesCount = m_editorScene->getSelectedEdges().size();

	if (nodesCount == 1 && edgesCount == 0 && sceneActions)
	{
		sceneActions->editNodeId(m_editorScene->getSelectedNodes().first());
		return;
	}

	if (nodesCount == 0 && edgesCount == 1 && sceneActions)
	{
		sceneActions->editEdgeId(m_editorScene->getSelectedEdges().first());
		return;
	}
}

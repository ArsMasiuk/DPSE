/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "qvge/CNode.h"
#include "qvge/CFileSerializerGEXF.h"
#include "qvge/CFileSerializerGraphML.h"
#include "qvge/CFileSerializerXGR.h"
#include "qvge/CEditorView.h"
#include "qvge/CImageExport.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QInputDialog>
#include <QMessageBox>

#include "CAttributeEditor.h"
#include "CItemAttributeEditor.h"
#include "CCommutationTable.h"
#include "CSceneOptionsDialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CBranchEditorScene.h"
#include "CFileSerializerGR.h"

#include "simu/mpisch/CMPISimulatorDialog.h"
#include "simu/graph_om/CGraphSimulatorDialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

	QApplication::setOrganizationName("DonNTU");
	QApplication::setApplicationName("POSEDS");

	ui->actionNew->setShortcut(QKeySequence::New);
	ui->actionOpen->setShortcut(QKeySequence::Open);
	ui->actionSave->setShortcut(QKeySequence::Save);
	ui->actionExit->setShortcut(QKeySequence::Close);

	// scene & view
    m_editorScene = new CBranchEditorScene(this);
	m_editorView = new CEditorView(m_editorScene, this);
    setCentralWidget(m_editorView);

	// status
	statusBar()->addPermanentWidget(m_countSection = new QLabel());

	// dockers
	QDockWidget* attrEditorDock = new QDockWidget(this);
	attrEditorDock->setWidget(m_attrEditor = new CAttributeEditor(this));
	attrEditorDock->setWindowTitle(tr("Graph Attributes"));
	addDockWidget(Qt::LeftDockWidgetArea, attrEditorDock);
	m_attrEditor->setScene(m_editorScene);

    QDockWidget* attrItemEditorDock = new QDockWidget(this);
    attrItemEditorDock->setWidget(m_attrItemEditor = new CItemAttributeEditor(this));
    attrItemEditorDock->setWindowTitle(tr("Item Attributes"));
    addDockWidget(Qt::RightDockWidgetArea, attrItemEditorDock);
    m_attrItemEditor->setScene(m_editorScene);

	QDockWidget* attrCommTableDock = new QDockWidget(this);
	auto commTable = new CCommutationTable(this);
	attrCommTableDock->setWidget(commTable);
	attrCommTableDock->setWindowTitle(tr("Commutations"));
	addDockWidget(Qt::RightDockWidgetArea, attrCommTableDock);
	commTable->setScene(m_editorScene);

    // actions
    ui->actionShowGrid->setChecked(m_editorScene->gridEnabled());
    connect(ui->actionShowGrid, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGrid(bool)));

    ui->actionSnapGrid->setChecked(m_editorScene->gridSnapEnabled());
    connect(ui->actionSnapGrid, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGridSnap(bool)));

	ui->actionShowLabels->setChecked(m_editorScene->itemLabelsEnabled());
	connect(ui->actionShowLabels, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableItemLabels(bool)));

    QActionGroup *modesGroup = new QActionGroup(this);
    modesGroup->setExclusive(true);
    modesGroup->addAction(ui->actionBranches);
    modesGroup->addAction(ui->actionFans);

    ui->actionBranches->setChecked(true);
    connect(ui->actionBranches, SIGNAL(triggered()), m_editorScene, SLOT(setBranchesMode()));
    connect(ui->actionFans, SIGNAL(triggered()), m_editorScene, SLOT(setFansMode()));

	connect(ui->actionUndo, SIGNAL(triggered()), m_editorScene, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), m_editorScene, SLOT(redo()));
	connect(m_editorScene, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
	connect(m_editorScene, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

	connect(ui->actionCopy, SIGNAL(triggered()), m_editorScene, SLOT(copy()));
	connect(ui->actionPaste, SIGNAL(triggered()), m_editorScene, SLOT(paste()));

	connect(m_editorScene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()));
	connect(m_editorView, SIGNAL(scaleChanged(double)), this, SLOT(onZoomChanged(double)));

	// settings
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (!geometry.isEmpty()) {
		restoreGeometry(geometry);
	}

	if (settings.value("maximized", true).toBool())
		showMaximized();
	else
		showNormal();

	m_lastFileName = settings.value("lastDir").toString();

	// new scene by default
	on_actionNew_triggered();

	onZoomChanged(1);
}

MainWindow::~MainWindow()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("geometry", saveGeometry());
	settings.setValue("maximized", isMaximized());
	settings.setValue("lastDir", QDir(m_lastFileName).canonicalPath());

    delete ui;
}


// actions

void MainWindow::on_actionZoom_triggered()
{
	m_editorView->zoomBy(1.3);
}

void MainWindow::on_actionUnzoom_triggered()
{
	m_editorView->zoomBy(1.0 / 1.3);
}

void MainWindow::on_actionResetZoom_triggered()
{
	m_editorView->zoomTo(1.0);
}

void MainWindow::on_actionFitToView_triggered()
{
	m_editorView->fitToView();
}

void MainWindow::onZoomChanged(double currentZoom)
{
	ui->actionResetZoom->setText(QString("%1%").arg((int)(currentZoom*100)));
}

void MainWindow::on_actionFactorNodes_triggered()
{
	bool ok;
	double factor = QInputDialog::getDouble(NULL, tr("Scale Factor"),
		tr("Scale node(s) coordinates by:"), 2, 0.0001, 10000.0, 4, &ok);

	if (ok)
	{
		QList<CNode*> nodes = m_editorScene->getItems<CNode>();
		for (auto node : nodes)
		{
			node->setPos(node->pos() * factor);
		}

		m_editorScene->addUndoState();
	}
}

void MainWindow::on_actionFind_triggered()
{
	QString findText = QInputDialog::getText(NULL, tr("Find items"), tr("Enter item(s) id text to find and select:"));
	if (findText.isEmpty())
		return;

	m_editorScene->deselectAll();

	QRegExp re;
	re.setPatternSyntax(QRegExp::Wildcard);
	re.setPattern(findText);
	re.setCaseSensitivity(Qt::CaseInsensitive);

	int count = 0;
	auto items = m_editorScene->getItems();
	for (auto item : items)
	{
		if (re.exactMatch(item->getId()))
		{
			item->getSceneItem()->setSelected(true);

			if (!count++)
				item->getSceneItem()->ensureVisible();
		}
	}

	if (count)
		QMessageBox::information(NULL, tr("Items found"), tr("%1 item(s) found").arg(count));
	else
		QMessageBox::warning(NULL, tr("Items not found"), tr("No matching item(s) found"));
}

void MainWindow::on_actionOptions_triggered()
{
	CSceneOptionsDialog d;
	if (d.exec(*m_editorScene) == QDialog::Accepted)
	{
		// this is to revise...
		updateSceneActions();
	}
}

void MainWindow::updateSceneActions()
{
	ui->actionShowGrid->setChecked(m_editorScene->gridEnabled());
	ui->actionSnapGrid->setChecked(m_editorScene->gridSnapEnabled());
	ui->actionShowLabels->setChecked(m_editorScene->itemLabelsEnabled());
}

void MainWindow::on_actionNew_triggered()
{
	m_editorScene->reset();
	m_editorScene->addUndoState();

	m_editorView->zoomTo(1.0);
}

void MainWindow::on_actionSave_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "Save Scene", m_lastFileName, "*.xgr");
	if (!fileName.isEmpty())
	{
		if (CFileSerializerXGR().save(fileName, *m_editorScene))
		{
			m_lastFileName = fileName;
		}
	}
}

void MainWindow::on_actionExport_triggered()
{
	CImageExport::write(*m_editorScene, m_lastFileName);
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "Open Scene", m_lastFileName, "*.xgr | *.gr | *.gexf | *.graphml");
	if (!fileName.isEmpty())
	{
		if (QFileInfo(fileName).suffix() == "gr")
		{
			CFileSerializerGR().load(fileName, *m_editorScene);

			m_editorScene->addUndoState();
			m_editorView->zoomTo(1.0);

			m_lastFileName = fileName;
			return;
		}

		if (QFileInfo(fileName).suffix() == "gexf")
		{
			CFileSerializerGEXF().load(fileName, *m_editorScene);

			m_editorScene->addUndoState();
			m_editorView->zoomTo(1.0);

			m_lastFileName = fileName;
			return;
		}

		if (QFileInfo(fileName).suffix() == "graphml")
		{
			CFileSerializerGraphML().load(fileName, *m_editorScene);

			m_editorScene->addUndoState();
			m_editorView->zoomTo(1.0);

			m_lastFileName = fileName;
			return;
		}

		// default (xgr)
		if (CFileSerializerXGR().load(fileName, *m_editorScene))
		{
			// this is to revise...
			updateSceneActions();

			m_editorScene->addUndoState();
			m_editorView->zoomTo(1.0);

			m_lastFileName = fileName;
			return;
		}
	}
}

void MainWindow::on_actionExit_triggered()
{
	QApplication::exit(0);
}


// callbacks

void MainWindow::onSceneChanged()
{
	auto items = m_editorScene->items();

	int edges = 0, nodes = 0;
	for (auto item : items)
	{
		if (dynamic_cast<CNode*>(item) != NULL)
		{
			nodes++;
			continue;
		}

		if (dynamic_cast<CConnection*>(item) != NULL)
		{
			edges++;
			continue;
		}
	}

	m_countSection->setText(tr("Items: %1 [Edges: %2 | Nodes: %3]").arg(items.count()).arg(edges).arg(nodes));
}


// simu

void MainWindow::on_actionMPI_Sch_triggered()
{
    CMPISimulatorDialog mpisim;

    if (!mpisim.run(*m_editorScene))
    {
        QMessageBox::critical(NULL, tr("Error running simulation"), tr("Cannot start MPI_Sch simulator :("));
    }
}

void MainWindow::on_actionGraph_OM_triggered()
{
	CGraphSimulatorDialog grsim(this);

	if (!grsim.run(*m_editorScene))
	{
		QMessageBox::critical(NULL, tr("Error running simulation"), tr("Cannot start Graph_OM simulator :("));
	}
}


// test

void MainWindow::on_actionImportTest_triggered()
{
	QFile dds("g:\\Doctor\\DPSE\\dpss\\data\\test\\Graph-117_DDS.txt");
	dds.open(QFile::ReadOnly);
	while (!dds.atEnd())
	{
		QByteArray ba = dds.readLine().simplified();
		auto data = ba.split(' ');
		auto id = data[2];
		auto k = data[3];
		auto edges = m_editorScene->getItemsById<CConnection>(id);
		if (edges.isEmpty()) {
			return;
		}
		edges.first()->setAttribute("K", k);
	}
}


// about

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, tr("About POSE-DS"), 
		tr("Problem oriented simulation environment\n"
			"Dialog subsystem\n\n"
			"Donetsk National Technical University\n"
			"Chair of Computing Systems\n\n"
			"(c) Dipl.eng. A.L.Masyuk 2002-2017\n"
			"Research supervisor: Dr.Dr.Prof. V.A.Svjatnyj\n"
			"Airing simulator code: Dr. O.M.Miroshkin\n"
		));
}

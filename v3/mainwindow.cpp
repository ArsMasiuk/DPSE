#include "qvge/CNode.h"
#include "qvge/CFileSerializerGEXF.h"
#include "qvge/CFileSerializerGraphML.h"
#include "qvge/CEditorView.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QInputDialog>

#include "CAttributeEditor.h"
#include "CItemAttributeEditor.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "CBranchEditorScene.h"
#include "TestItems.h"
#include "CFileSerializerGR.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

	connect(ui->actionBranches, SIGNAL(triggered()), this, SLOT(setBranchesMode()));
	connect(ui->actionFans, SIGNAL(triggered()), this, SLOT(setFansMode()));

	connect(ui->actionUndo, SIGNAL(triggered()), m_editorScene, SLOT(undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), m_editorScene, SLOT(redo()));
	connect(m_editorScene, SIGNAL(undoAvailable(bool)), ui->actionUndo, SLOT(setEnabled(bool)));
	connect(m_editorScene, SIGNAL(redoAvailable(bool)), ui->actionRedo, SLOT(setEnabled(bool)));

	connect(m_editorScene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()));

	// new scene by default
	on_actionNew_triggered();

/*
    // test
    CTestRoundNode *node1 = new CTestRoundNode();
    node1->setPos(210,210);
    m_editorScene->addItem(node1);
	//node1->setEnabled(false);

    CTestRectNode *node2 = new CTestRectNode();
    node2->setPos(100,50);
    m_editorScene->addItem(node2);
	node2->setFlag(CRectNode::ItemIsSelectable, false);

	CBranchNode *node3 = new CBranchNode();
	node3->setPos(200, 150);
	m_editorScene->addItem(node3);

	CRoundNode *node4 = new CRoundNode();
	node4->setPos(250, 150);
	m_editorScene->addItem(node4);
	node4->setBrush(Qt::red);
	node4->setPen(QPen(Qt::darkRed, 2));


 //   CConnection *conn1 = new CConnection();
 //   conn1->setFirstNode(node1);
 //   conn1->setLastNode(node2);
 //   m_editorScene->addItem(conn1);

	QGraphicsLineItem *line = new QGraphicsLineItem();
	line->setLine(QLineF(0, 0, 300, 300));
	line->setFlags(QGraphicsItem::ItemIsSelectable);
	m_editorScene->addItem(line);

	QGraphicsEllipseItem *el = new QGraphicsEllipseItem();
	el->setPos(0, 0);
	el->setRect(-30, -20, 30, 20);
	el->setFlags(QGraphicsItem::ItemIsSelectable);
	m_editorScene->addItem(el);


	// default item
	typedef TItem<QGraphicsEllipseItem, CItem> EllipseItem;

	auto *i1 = new EllipseItem();
	i1->setPos(300, 300);
	i1->setRect(-30, -20, 30, 20);
	i1->setPen(QPen(Qt::blue, 2));
	i1->setBrush(Qt::yellow);
	m_editorScene->addItem(i1);

	// default item with custom selection
	auto *i2 = new EllipseItem();
	i2->setPos(340, 300);
	i2->setRect(-30, -20, 30, 20);
	i2->setPen(QPen(Qt::blue, 2));
	i2->setBrush(Qt::magenta);
	m_editorScene->addItem(i2);
	i2->setItemFlag(IF_FramelessSelection);
	*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionZoom_triggered()
{
	m_editorView->scale(2, 2);
}

void MainWindow::on_actionUnzoom_triggered()
{
	m_editorView->scale(0.5, 0.5);
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

void MainWindow::on_actionNew_triggered()
{
	m_editorScene->reset();
	m_editorScene->activate();
}

void MainWindow::on_actionSave_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "Save Scene", m_lastFileName, "*.xgr");
	if (!fileName.isEmpty())
	{
		QFile saveFile(fileName);
		if (saveFile.open(QFile::WriteOnly))
		{
			QDataStream ds(&saveFile);
			m_editorScene->storeTo(ds);

			m_lastFileName = fileName;
		}
	}
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(NULL, "Open Scene", m_lastFileName, "*.xgr | *.gr | *.gexf | *.graphml");
	if (!fileName.isEmpty())
	{
		if (QFileInfo(fileName).suffix() == "gr")
		{
			CFileSerializerGR().load(fileName, *m_editorScene);
			return;
		}

		if (QFileInfo(fileName).suffix() == "gexf")
		{
			CFileSerializerGEXF().load(fileName, *m_editorScene);
			return;
		}

		if (QFileInfo(fileName).suffix() == "graphml")
		{
			CFileSerializerGraphML().load(fileName, *m_editorScene);
			return;
		}

		// default (xgr)
		QFile openFile(fileName);
		if (openFile.open(QFile::ReadOnly))
		{
			m_editorScene->reset();

			QDataStream ds(&openFile);
			m_editorScene->restoreFrom(ds);

			m_editorScene->activate();

			m_lastFileName = fileName;
		}
	}
}


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



#include "CBranchEditorScene.h"
#include "CNode.h"
//#include "CConnection.h"

#include <QGraphicsView>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_editorScene = new CBranchEditorScene(this);
    m_editorView = new QGraphicsView(this);
    m_editorView->setScene(m_editorScene);

    setCentralWidget(m_editorView);

    // actions
    ui->actionShowGrid->setChecked(m_editorScene->gridEnabled());
    connect(ui->actionShowGrid, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGrid(bool)));

    ui->actionSnapGrid->setChecked(m_editorScene->gridSnapEnabled());
    connect(ui->actionSnapGrid, SIGNAL(toggled(bool)), m_editorScene, SLOT(enableGridSnap(bool)));

    QActionGroup *modesGroup = new QActionGroup(this);
    modesGroup->setExclusive(true);
    modesGroup->addAction(ui->actionSelection);
    modesGroup->addAction(ui->actionBranches);
    modesGroup->addAction(ui->actionFans);

    ui->actionSelection->setChecked(true);
    connect(ui->actionSelection, SIGNAL(triggered()), m_editorScene, SLOT(setSelectionMode()));
    connect(ui->actionBranches, SIGNAL(triggered()), m_editorScene, SLOT(setBranchesMode()));
    connect(ui->actionFans, SIGNAL(triggered()), m_editorScene, SLOT(setFansMode()));

	connect(ui->actionSelection, SIGNAL(triggered()), this, SLOT(setSelectionMode()));
	connect(ui->actionBranches, SIGNAL(triggered()), this, SLOT(setBranchesMode()));
	connect(ui->actionFans, SIGNAL(triggered()), this, SLOT(setFansMode()));

    // test
    CRoundNode *node1 = new CRoundNode();
    node1->setPos(210,210);
    m_editorScene->addItem(node1);

    CRectNode *node2 = new CRectNode();
    node2->setPos(100,50);
    m_editorScene->addItem(node2);
	////node2->setEnabled(false);

	CBranchNode *node3 = new CBranchNode();
	node3->setPos(200, 150);
	m_editorScene->addItem(node3);

 //   CConnection *conn1 = new CConnection();
 //   conn1->setFirstNode(node1);
 //   conn1->setLastNode(node2);
 //   m_editorScene->addItem(conn1);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setSelectionMode()
{
	m_editorView->setCursor(Qt::ArrowCursor);
}

void MainWindow::setBranchesMode()
{
	//m_editorView->setCursor(Qt::CrossCursor);
}

void MainWindow::setFansMode()
{}

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
	QString fileName = QFileDialog::getOpenFileName(NULL, "Open Scene", m_lastFileName, "*.xgr");
	if (!fileName.isEmpty())
	{
		QFile openFile(fileName);
		if (openFile.open(QFile::ReadOnly))
		{
			QDataStream ds(&openFile);
			m_editorScene->restoreFrom(ds);

			m_lastFileName = fileName;
		}
	}
}


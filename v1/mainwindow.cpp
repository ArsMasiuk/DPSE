#include "CBranchEditorView.h"
#include "CEditorScene.h"
#include "CNode.h"
#include "CConnection.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_editorScene = new CEditorScene(this);
    m_editorView = new CBranchEditorView(this);
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
    connect(ui->actionSelection, SIGNAL(triggered()), m_editorView, SLOT(setSelectionMode()));
    connect(ui->actionBranches, SIGNAL(triggered()), m_editorView, SLOT(setBranchesMode()));
    connect(ui->actionFans, SIGNAL(triggered()), m_editorView, SLOT(setFansMode()));

    // test
    CRoundNode *node1 = new CRoundNode();
    node1->setPos(10,10);
    m_editorScene->addItem(node1);

    CRectNode *node2 = new CRectNode();
    node2->setPos(100,50);
    m_editorScene->addItem(node2);
	//node2->setEnabled(false);

    CConnection *conn1 = new CConnection();
    conn1->setFirstNode(node1);
    conn1->setLastNode(node2);
    m_editorScene->addItem(conn1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

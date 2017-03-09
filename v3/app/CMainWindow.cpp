#include "CMainWindow.h"
#include "CTabController.h"
#include "CDocumentController.h"
#include "IDocument.h"
#include "CTextDocument.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QAction>


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    createMDI();

    createMainMenu();

    // customization
    m_documentController->addDocumentFactory(new CTextDocument());
}

CMainWindow::~CMainWindow()
{
}


// protected

void CMainWindow::createMDI()
{
    m_documentController = new CDocumentController(this);

    m_tabController = new CTabController(this);
    setCentralWidget(m_tabController);

    connect(m_documentController, SIGNAL(documentFactoryAdded(IDocument*)),
        this, SLOT(onDocumentFactoryAdded(IDocument*)));

    connect(m_documentController, SIGNAL(documentCreated(IDocument*)),
        m_tabController, SLOT(onDocumentCreated(IDocument*)));
}


void CMainWindow::createMainMenu()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));

    m_newMenu = new QMenu(tr("&New"));
    QAction *newDocument = m_fileMenu->addMenu(m_newMenu);
    m_newMenu->setEnabled(false);
    m_newActions = new QActionGroup(this);
    connect(m_newActions, SIGNAL(triggered(QAction*)), this, SLOT(on_actionNew_triggered(QAction*)));

    QAction *openDocument = m_fileMenu->addAction(tr("&Open..."), this, SLOT(on_actionOpen_triggered()));
    openDocument->setEnabled(false);

    QAction *saveDocument = m_fileMenu->addAction(tr("&Save"), this, SLOT(on_actionSave_triggered()));
    saveDocument->setEnabled(false);

    QAction *saveAsDocument = m_fileMenu->addAction(tr("Save As..."), this, SLOT(on_actionSave_triggered()));
    saveAsDocument->setEnabled(false);

    m_fileMenu->addSeparator();

    QAction *exitApp = m_fileMenu->addAction(tr("E&xit"), this, SLOT(close()));
}


// callbacks

void CMainWindow::onDocumentFactoryAdded(IDocument *factory)
{
    QAction *newAction = new QAction(factory->name());
    newAction->setData(factory->id());
    m_newMenu->addAction(newAction);
    m_newActions->addAction(newAction);

    m_newMenu->setEnabled(true);
}


// actions

void CMainWindow::on_actionSave_triggered()
{
}

void CMainWindow::on_actionOpen_triggered()
{
}

void CMainWindow::on_actionNew_triggered(QAction* action)
{
    QByteArray factoryId = action->data().toByteArray();

    m_documentController->createDocument(factoryId);
}

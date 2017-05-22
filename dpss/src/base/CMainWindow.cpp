#include "CMainWindow.h"

#include <QFileDialog>
#include <QDockWidget>
#include <QStatusBar>
#include <QMenuBar>
#include <QInputDialog>
#include <QTimer>


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_appName("MyApp"),
    m_isChanged(false)
{
    QTimer::singleShot(0, this, SLOT(initUI()));
}

CMainWindow::~CMainWindow()
{
}


// protected

void CMainWindow::initUI()
{
    createMainMenu();

    createFileToolbar();

    updateTitle();
}


void CMainWindow::createMainMenu()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));

//    m_newMenu = new QMenu(tr("&New"));
//    m_newDocument = m_fileMenu->addMenu(m_newMenu);
//    m_newDocument->setIcon(QIcon(":/Icons/New"));
//    m_newMenu->setEnabled(false);

    m_newDocument = m_fileMenu->addAction(QIcon(":/Icons/New"), tr("&New..."));
    m_newDocument->setEnabled(false);

    m_openDocument = m_fileMenu->addAction(QIcon(":/Icons/Open"), tr("&Open..."), this, SLOT(on_actionOpen_triggered()));
    m_openDocument->setEnabled(false);

    m_saveDocument = m_fileMenu->addAction(QIcon(":/Icons/Save"), tr("&Save"), this, SLOT(on_actionSave_triggered()));
    m_saveDocument->setEnabled(false);

    m_saveAsDocument = m_fileMenu->addAction(tr("Save As..."), this, SLOT(on_actionSaveAs_triggered()));
    m_saveAsDocument->setEnabled(false);

    m_fileMenu->addSeparator();

    /*QAction *exitApp =*/ m_fileMenu->addAction(tr("E&xit"), this, SLOT(close()));
}


void CMainWindow::createFileToolbar()
{
    QToolBar *fileToolbar = addToolBar(tr("File"));
    fileToolbar->addAction(m_newDocument);
    fileToolbar->addAction(m_openDocument);
    fileToolbar->addAction(m_saveDocument);
    //fileToolbar->addAction(m_saveAsDocument);
}


void CMainWindow::updateTitle()
{
    QString docName = m_currentFileName;

    if (m_currentFileName.isEmpty())
        docName = "New File";

    if (m_isChanged)
        docName += "*";

    setWindowTitle(QString("%1 - %2").arg(docName, m_appName));
}


void CMainWindow::onDocumentChanged()
{
    m_isChanged = true;

    updateTitle();
}


// actions

void CMainWindow::on_actionOpen_triggered()
{
    QString title = tr("Open File");
    QString filter = tr("Any File (*.*)");
    onOpenDocumentDialog(title, filter);

    QString fileName = QFileDialog::getOpenFileName(NULL, title, m_lastFileName, filter);
    if (fileName.isEmpty())
        return;



    updateTitle();
}


void CMainWindow::on_actionSave_triggered()
{
}


void CMainWindow::on_actionSaveAs_triggered()
{
}


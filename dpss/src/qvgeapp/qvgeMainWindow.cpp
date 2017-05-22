#include "qvgeMainWindow.h"


qvgeMainWindow::qvgeMainWindow()
{
//    QMenu *newActions = new QMenu();
//    m_newDocument->setMenu(newActions);
//    QAction *newGraph = newActions->addAction(tr("New Graph"));
//    QAction *newGraph2 = newActions->addAction(tr("New Graph 2"));

    //QAction *newGraph = m_newDocument->addAction(tr("New Graph"), this, SLOT(newGraphDocument()));

}


void qvgeMainWindow::initUI()
{
    Super::initUI();

    statusBar();

    // actions
    m_newDocument->setText(tr("New Graph"));
    connect(m_newDocument, &QAction::triggered, this, &qvgeMainWindow::newGraphDocument);
    m_newDocument->setEnabled(true);

    m_openDocument->setEnabled(true);

    // default
    createDefaultDocument();
}


void qvgeMainWindow::createDefaultDocument()
{
    // scene
    m_editorScene = new CNodeEditorScene(this);
    m_editorView = new CEditorView(m_editorScene, this);
    setCentralWidget(m_editorView);

    connect(m_editorScene, &CEditorScene::sceneChanged, this, &CMainWindow::onDocumentChanged);
}


void qvgeMainWindow::newGraphDocument()
{

}


void qvgeMainWindow::onOpenDocumentDialog(QString &title, QString &filter)
{
    title = tr("Open Graph Document");
    filter = tr("Any supported format") + " (*.gexf *.graphml)";
    filter += ";;GEXF (*.gexf);;GraphML (*.graphml)";
}

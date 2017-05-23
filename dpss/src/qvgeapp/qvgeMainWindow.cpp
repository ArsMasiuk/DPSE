#include "qvgeMainWindow.h"

#include <QtWidgets/QPlainTextEdit>


qvgeMainWindow::qvgeMainWindow()
{
    m_appName = "Qt Visual Graph Editor";

    CDocumentFormat gexf = { "GEXF", "*.gexf", false, true };
    CDocumentFormat graphml = { "GraphML", "*.graphml", true, true };
//    CDocumentFormat gr = { "Old plain GR", "*.gr", false, true };
//    CDocumentFormat xgr = { "XML Graph", "*.xgr", true, true };
    CDocument graph = { tr("Graph Document"), tr("Directed or undirected graph"), "graph", true, {gexf, graphml} };
    addDocument(graph);

    CDocumentFormat txt = { tr("Plain Text"), "*.txt", true, true };
    CDocument text = { tr("Text Document"), tr("Simple text document"), "text", true, {txt} };
    addDocument(text);
}


void qvgeMainWindow::initUI()
{
    Super::initUI();

    statusBar();
}


bool qvgeMainWindow::onCreateNewDocument(const CDocument& doc)
{
    // scene
    if (doc.type == "graph")
    {
        m_editorScene = new CNodeEditorScene(this);
        m_editorView = new CEditorView(m_editorScene, this);
        setCentralWidget(m_editorView);

        connect(m_editorScene, &CEditorScene::sceneChanged, this, &CMainWindow::onDocumentChanged);
        return true;
    }

    // text
    if (doc.type == "text")
    {
        QPlainTextEdit *textEditor = new QPlainTextEdit(this);
        setCentralWidget(textEditor);

        connect(textEditor, &QPlainTextEdit::textChanged, this, &CMainWindow::onDocumentChanged);
        return true;
    }

    // unknown type
    return false;
}


void qvgeMainWindow::onOpenDocumentDialog(QString &title, QString &filter)
{
    Super::onOpenDocumentDialog(title, filter);

    title = tr("Open Graph Document");
}

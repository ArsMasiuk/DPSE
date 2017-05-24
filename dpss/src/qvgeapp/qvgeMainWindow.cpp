#include "qvgeMainWindow.h"

#include <QFile>
#include <QTextStream>


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


void qvgeMainWindow::init(int argc, char *argv[])
{
    Super::init(argc, argv);

    statusBar()->showMessage(tr("qvge started."));
}


bool qvgeMainWindow::onCreateNewDocument(const QByteArray &docType)
{
    // scene
    if (docType == "graph")
    {
        m_editorScene = new CNodeEditorScene(this);
        m_editorView = new CEditorView(m_editorScene, this);
        setCentralWidget(m_editorView);

        connect(m_editorScene, &CEditorScene::sceneChanged, this, &CMainWindow::onDocumentChanged);
        return true;
    }

    // text
    if (docType == "text")
    {
        m_textEditor = new QPlainTextEdit(this);
        setCentralWidget(m_textEditor);

        connect(m_textEditor, &QPlainTextEdit::textChanged, this, &CMainWindow::onDocumentChanged);
        return true;
    }

    // unknown type
    return false;
}


bool qvgeMainWindow::onOpenDocument(const QString &fileName, QByteArray &docType)
{
    if (fileName.toLower().endsWith(".txt"))
    {
        docType = "text";

        if (onCreateNewDocument(docType))
        {
            QFile f(fileName);
            if (f.open(QFile::ReadOnly))
            {
                QTextStream ts(&f);
                m_textEditor->setPlainText(ts.readAll());
                f.close();
                return true;
            }
        }
    }

    return false;
}

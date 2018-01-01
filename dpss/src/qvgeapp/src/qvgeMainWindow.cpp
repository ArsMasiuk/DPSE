#include "qvgeMainWindow.h"
#include "qvgeNodeEditorUIController.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QFileInfo>

#include <qvge/CFileSerializerGEXF.h>
#include <qvge/CFileSerializerGraphML.h>
#include <qvge/CFileSerializerXGR.h>


qvgeMainWindow::qvgeMainWindow()
{
    QApplication::setApplicationName("Qt Visual Graph Editor");

    CDocumentFormat gexf = { "GEXF", "*.gexf", false, true };
    CDocumentFormat graphml = { "GraphML", "*.graphml", true, true };
//    CDocumentFormat gr = { "Old plain GR", "*.gr", false, true };
    CDocumentFormat xgr = { "XML Graph", "*.xgr", true, true };
    CDocument graph = { tr("Graph Document"), tr("Directed or undirected graph"), "graph", true, {gexf, graphml, xgr} };
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

        /*auto uiController = */new qvgeNodeEditorUIController(this, m_editorScene, m_editorView);

        // restore settings for this instance
        readSettings();

        return true;
    }

    // text
    if (docType == "text")
    {
        m_textEditor = new QPlainTextEdit(this);
        setCentralWidget(m_textEditor);

        connect(m_textEditor, &QPlainTextEdit::textChanged, this, &CMainWindow::onDocumentChanged);

        // restore settings for this instance
        readSettings();

        return true;
    }

    // unknown type
    return false;
}


bool qvgeMainWindow::onOpenDocument(const QString &fileName, QByteArray &docType)
{
	// graphs formats
    if (fileName.toLower().endsWith(".graphml"))
    {
        docType = "graph";

        if (onCreateNewDocument(docType))
        {
			if (CFileSerializerGraphML().load(fileName, *m_editorScene))
			{
				m_editorScene->addUndoState();
				return true;
			}
        }

        return false;
    }


    if (fileName.toLower().endsWith(".gexf"))
    {
        docType = "graph";

        if (onCreateNewDocument(docType))
        {
			if (CFileSerializerGEXF().load(fileName, *m_editorScene))
			{
				m_editorScene->addUndoState();
				return true;
			}
        }
        
		return false;
    }


    if (fileName.toLower().endsWith(".xgr"))
    {
        docType = "graph";

        if (onCreateNewDocument(docType))
        {
            if (CFileSerializerXGR().load(fileName, *m_editorScene))
            {
                m_editorScene->addUndoState();
                return true;
            }
        }

        return false;
    }

	// fallback: load as text
	//if (fileName.toLower().endsWith(".txt"))
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


bool qvgeMainWindow::onSaveDocument(const QString &fileName, const QString &/*selectedFilter*/, const QByteArray &docType)
{
    // text
    if (docType == "text")
    {
        QFile f(fileName);
        if (f.open(QFile::WriteOnly))
        {
            QTextStream ts(&f);
            ts << m_textEditor->toPlainText();
            f.close();
            return true;
        }
        return false;
    }

    // graph
	if (docType == "graph")
	{
		QString extType = QFileInfo(fileName).suffix().toLower();

		if (extType == "xgr")
		{
			if (CFileSerializerXGR().save(fileName, *m_editorScene))
			{
				return true;
			}

			return false;
		}
	}


    // unknown type
    return false;
}



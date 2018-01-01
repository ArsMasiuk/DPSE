#ifndef QVGEMAINWINDOW_H
#define QVGEMAINWINDOW_H

#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>

#include <base/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>


class qvgeMainWindow : public CMainWindow
{
    Q_OBJECT

public:
    typedef CMainWindow Super;

    qvgeMainWindow();

    virtual void init(int argc, char *argv[]);

protected:
    virtual bool onCreateNewDocument(const QByteArray &docType);
    virtual bool onOpenDocument(const QString &fileName, QByteArray &docType);
    virtual bool onSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType);
	
private:
    CNodeEditorScene *m_editorScene;
    CEditorView *m_editorView;

    QPlainTextEdit *m_textEditor;
};

#endif // QVGEMAINWINDOW_H

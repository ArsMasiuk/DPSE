#ifndef QVGEMAINWINDOW_H
#define QVGEMAINWINDOW_H

#include <base/CMainWindow.h>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CEditorView.h>


class qvgeMainWindow : public CMainWindow
{
    Q_OBJECT

public:
    typedef CMainWindow Super;

    qvgeMainWindow();

protected Q_SLOTS:
    virtual void initUI();

    void newGraphDocument();

protected:
    virtual void createDefaultDocument();
    virtual void onOpenDocumentDialog(QString &title, QString &filter);

private:
    CNodeEditorScene *m_editorScene;
    CEditorView *m_editorView;
};

#endif // QVGEMAINWINDOW_H

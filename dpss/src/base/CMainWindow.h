#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QActionGroup>
#include <QToolBar>
#include <QSet>
#include <QByteArray>
#include <QString>


struct CDocumentFormat
{
    QString name;
    QString filters;
    bool canSave, canRead;
};


struct CDocument
{
    QString name;
    QString description;
    QByteArray type;
    bool canCreate;
    QList<CDocumentFormat> formats;
};


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

    void addDocument(const CDocument& doc);

protected:
    virtual void createMainMenu();
    virtual void fillNewFileMenu();
    virtual void createFileToolbar();

    virtual void updateTitle();

    virtual void doCreateNewDocument(const CDocument& doc);
    virtual bool onCreateNewDocument(const CDocument& doc);

    virtual void onOpenDocumentDialog(QString &title, QString &filter);
    virtual void onSaveDocumentDialog(QString &title, QString &filter);

protected Q_SLOTS:
    virtual void initUI();

    void createNewDocument();
    void createNewDocument(QAction*);

    virtual void on_actionOpen_triggered();
    virtual void on_actionSave_triggered();
    virtual void on_actionSaveAs_triggered();

public Q_SLOTS:
    virtual void onDocumentChanged();

protected:
    QString m_appName;

    QMenu *m_fileMenu;
    QMenu *m_newMenu;
    QAction *m_newDocument;
    QAction *m_openDocument;
    QAction *m_saveDocument;
    QAction *m_saveAsDocument;

	QString m_lastFileName;
    QString m_currentFileName;
    bool m_isChanged;
    QByteArray m_currentDocType;

    QMap<QByteArray, CDocument> m_docTypes;
    QList<QByteArray> m_docTypeCreate;
};

#endif // MAINWINDOW_H

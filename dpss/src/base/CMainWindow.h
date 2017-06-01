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

    virtual void init(int argc, char *argv[]);

    void addDocument(const CDocument& doc);

protected:
    virtual void processParams(int argc, char *argv[]);

    virtual void createMainMenu();
    virtual void fillNewFileMenu();
    virtual void createFileToolbar();

    virtual void updateTitle();
    virtual void updateActions();

    virtual void doCreateNewDocument(const QByteArray &docType);
    virtual bool onCreateNewDocument(const QByteArray &docType);

    virtual void onOpenDocumentDialog(QString &title, QString &filter);
    virtual void doOpenDocument(const QString &fileName);
    virtual bool onOpenDocument(const QString &fileName, QByteArray &docType) { return false; }

    virtual void onSaveDocumentDialog(QString &title, QString &filter) {}
    virtual void doSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType);
    virtual bool onSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType) { return true; }

protected Q_SLOTS:
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

    QString m_currentFileName;
    QByteArray m_currentDocType;
    bool m_isChanged;

    QString m_lastOpenFilter, m_lastSaveFilter;

    QMap<QByteArray, CDocument> m_docTypes;
    QList<QByteArray> m_docTypeCreate;
};

#endif // MAINWINDOW_H

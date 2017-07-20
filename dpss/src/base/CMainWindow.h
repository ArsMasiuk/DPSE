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
    virtual ~CMainWindow();

    virtual void init(int argc, char *argv[]);

    void addDocument(const CDocument& doc);

public Q_SLOTS:
	virtual void onDocumentChanged();

protected:
	virtual void closeEvent(QCloseEvent *event);

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
    virtual bool doSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType);
    virtual bool onSaveDocument(const QString &fileName, const QString &selectedFilter, const QByteArray &docType) { return true; }

	virtual bool saveOnExit();
	virtual bool save();
	virtual bool saveAs();

    CMainWindow* findDocumentWindow(const QString &fileName);

	virtual void readSettings();
	virtual void writeSettings();

protected Q_SLOTS:
    void createNewDocument();
    void createNewDocument(QAction*);

    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();

protected:
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

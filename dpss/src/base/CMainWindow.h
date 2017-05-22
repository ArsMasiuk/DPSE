#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QActionGroup>
#include <QToolBar>


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

protected:
    virtual void createMainMenu();
    virtual void createFileToolbar();

    virtual void updateTitle();

    virtual void onOpenDocumentDialog(QString &title, QString &filter) = 0;

protected Q_SLOTS:
    virtual void initUI();

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
};

#endif // MAINWINDOW_H

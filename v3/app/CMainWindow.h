#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QActionGroup>


class CTabController;
class CDocumentController;
class IDocument;


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

protected:
    virtual void createMDI();
    virtual void createMainMenu();

protected Q_SLOTS:
	void on_actionSave_triggered();
	void on_actionOpen_triggered();
    void on_actionNew_triggered(QAction*);

    // callbacks
    void onDocumentFactoryAdded(IDocument *factory);

protected:
    CDocumentController *m_documentController;
    CTabController *m_tabController;

    QMenu *m_fileMenu;
    QMenu *m_newMenu;
    QActionGroup *m_newActions;

	QString m_lastFileName;
};

#endif // MAINWINDOW_H

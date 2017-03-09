#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
	void setSelectionMode();
	void setBranchesMode();
	void setFansMode();

	void on_actionSave_triggered();
	void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;

    class CBranchEditorScene* m_editorScene;
    class QGraphicsView* m_editorView;

	QString m_lastFileName;
};

#endif // MAINWINDOW_H

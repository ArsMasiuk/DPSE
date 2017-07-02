/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class CAttributeEditor;
class CEditorView;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
	void on_actionNew_triggered();
	void on_actionSave_triggered();
	void on_actionExport_triggered();
	void on_actionOpen_triggered();

	void on_actionZoom_triggered();
	void on_actionUnzoom_triggered();
	void on_actionResetZoom_triggered();
	void on_actionFitToView_triggered();

	void on_actionFactorNodes_triggered();

    //void on_actionUndoTest_triggered();
	void on_actionImportTest_triggered();

	void on_actionAbout_triggered();

	void onSceneChanged();
	void onZoomChanged(double currentZoom);

    // simu
    void on_actionMPI_Sch_triggered();
	void on_actionGraph_OM_triggered();

private:
    Ui::MainWindow *ui;
	class CAttributeEditor* m_attrEditor;
	class CItemAttributeEditor* m_attrItemEditor;

    class CBranchEditorScene* m_editorScene;
    class CEditorView* m_editorView;

	QLabel* m_countSection;

	QString m_lastFileName;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class CAttributeEditor;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
	void on_actionNew_triggered();
	void on_actionSave_triggered();
	void on_actionOpen_triggered();

	void on_actionZoom_triggered();
	void on_actionUnzoom_triggered();

	void on_actionFactorNodes_triggered();

	void on_actionUndoTest_triggered();

	void onSceneChanged();

private:
    Ui::MainWindow *ui;
	class CAttributeEditor* m_attrEditor;
	class CItemAttributeEditor* m_attrItemEditor;

    class CBranchEditorScene* m_editorScene;
    class QGraphicsView* m_editorView;

	QLabel* m_countSection;

	QString m_lastFileName;
};

#endif // MAINWINDOW_H

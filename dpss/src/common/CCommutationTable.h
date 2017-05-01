#ifndef CCommutationTable_H
#define CCommutationTable_H

#include <QWidget>

class CEditorScene;
class CNodeEditorScene;
struct CAttribute;

#include "ui_CCommutationTable.h"


class CCommutationTable : public QWidget
{
	Q_OBJECT

public:
	CCommutationTable(QWidget *parent = 0);
	~CCommutationTable();

	void setScene(CNodeEditorScene* scene);

protected:
	void connectSignals(CEditorScene* scene);
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

protected Q_SLOTS:
	void onSceneChanged();
	void onSelectionChanged();
	void on_Table_itemSelectionChanged();
	void on_Table_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	Ui::CCommutationTable ui;

	CNodeEditorScene *m_scene;
};

#endif // CCommutationTable_H

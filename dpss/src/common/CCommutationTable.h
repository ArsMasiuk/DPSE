#ifndef CCommutationTable_H
#define CCommutationTable_H

#include <QWidget>

class CEditorScene;
struct CAttribute;

#include "ui_CCommutationTable.h"


class CCommutationTable : public QWidget
{
	Q_OBJECT

public:
	CCommutationTable(QWidget *parent = 0);
	~CCommutationTable();

	void setScene(CEditorScene* scene);

protected:
	void connectSignals(CEditorScene* scene);
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

protected Q_SLOTS:
	void onSceneChanged();
	void onSelectionChanged();
	void on_Table_itemSelectionChanged();

private:
	Ui::CCommutationTable ui;

	CEditorScene *m_scene;
};

#endif // CCommutationTable_H

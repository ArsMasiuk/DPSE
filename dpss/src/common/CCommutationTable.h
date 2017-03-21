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
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

protected Q_SLOTS:
	void onSceneChanged();

private:
	Ui::CCommutationTable ui;

	CEditorScene *m_scene;
};

#endif // CCommutationTable_H

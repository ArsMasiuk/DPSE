#ifndef CITEMATTRIBUTEEDITOR_H
#define CITEMATTRIBUTEEDITOR_H

#include <QWidget>
#include "ui_CItemAttributeEditor.h"

class CEditorScene;
class CItem;


class CItemAttributeEditor : public QWidget
{
	Q_OBJECT

public:
	CItemAttributeEditor(QWidget *parent = 0);
	~CItemAttributeEditor();

	void setScene(CEditorScene* scene);

protected:
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

protected Q_SLOTS:
	void onSceneChanged();
	void onSelectionChanged();
    void on_Editor_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	void listAttributes(QTreeWidgetItem* rootItem, const QList<CItem*>& items, const QByteArray& classId);

	Ui::CItemAttributeEditor ui;

	CEditorScene *m_scene;
};

#endif // CITEMATTRIBUTEEDITOR_H

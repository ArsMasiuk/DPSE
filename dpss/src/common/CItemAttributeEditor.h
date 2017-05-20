/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CITEMATTRIBUTEEDITOR_H
#define CITEMATTRIBUTEEDITOR_H

#include <QWidget>
#include "ui_CItemAttributeEditor.h"

class CEditorScene;
class CNodeEditorScene;
class CItem;


class CItemAttributeEditor : public QWidget
{
	Q_OBJECT

public:
	CItemAttributeEditor(QWidget *parent = 0);
	~CItemAttributeEditor();

	void setScene(CNodeEditorScene* scene);

protected:
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

	virtual QVariant editValue(const QByteArray& classId, const QByteArray& attrId, const QVariant& attrValue);

protected Q_SLOTS:
	void onSceneChanged();
	void onSelectionChanged();
    void on_Editor_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();

private:
	void listAttributes(QTreeWidgetItem* rootItem, const QList<CItem*>& items, const QByteArray& classId);

	Ui::CItemAttributeEditor ui;

	CNodeEditorScene *m_scene;
};

#endif // CITEMATTRIBUTEEDITOR_H

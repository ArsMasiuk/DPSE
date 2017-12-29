/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CATTRIBUTEEDITOR_H
#define CATTRIBUTEEDITOR_H

#include <QWidget>

class CEditorScene;
struct CAttribute;

#include "ui_CAttributeEditor.h"


class CAttributeEditor : public QWidget
{
	Q_OBJECT

public:
	CAttributeEditor(QWidget *parent = 0);
	~CAttributeEditor();

	void setScene(CEditorScene* scene);

protected:
	void onSceneAttached(CEditorScene* scene);
	void onSceneDetached(CEditorScene* scene);

	void fillClassAttr(const QByteArray& classId, QTreeWidgetItem* classTreeItem);

protected Q_SLOTS:
	void onSceneChanged();
	void on_AttributeList_itemChanged(QTreeWidgetItem *item, int column);
	void on_AttributeList_itemDoubleClicked(QTreeWidgetItem *item, int column);
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();

private:
	Ui::CAttributeEditor ui;

	CEditorScene *m_scene;
};

#endif // CATTRIBUTEEDITOR_H

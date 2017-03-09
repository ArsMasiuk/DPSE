#ifndef CATTRIBUTEEDITOR_H
#define CATTRIBUTEEDITOR_H

#include <QWidget>

class CEditorScene;
class CAttribute;

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

private:
	Ui::CAttributeEditor ui;

	CEditorScene *m_scene;
};

#endif // CATTRIBUTEEDITOR_H

#ifndef CATTRIBUTESEDITORUI_H
#define CATTRIBUTESEDITORUI_H

#include <QWidget>
#include <QList>

class CEditorScene;
class CItem;

class CBaseProperty;

namespace Ui {
class CAttributesEditorUI;
}

class CAttributesEditorUI : public QWidget
{
    Q_OBJECT

public:
    explicit CAttributesEditorUI(QWidget *parent = 0);
    ~CAttributesEditorUI();

    void setupFromItems(CEditorScene& scene, QList<CItem*>& items);

private Q_SLOTS:
	void on_AddButton_clicked();
	void on_RemoveButton_clicked();
	void on_Editor_valueChanged(CBaseProperty* prop, const QVariant &v);

private:
    Ui::CAttributesEditorUI *ui;

    CEditorScene *m_scene;
	QList<CItem*> m_items;
};

#endif // CATTRIBUTESEDITORUI_H

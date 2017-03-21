#include "CAttributeEditor.h"
#include <qvge/CEditorScene.h>

CAttributeEditor::CAttributeEditor(QWidget *parent)
	: QWidget(parent),
	m_scene(NULL)
{
	ui.setupUi(this);
}

CAttributeEditor::~CAttributeEditor()
{

}

void CAttributeEditor::setScene(CEditorScene* scene)
{
	ui.ClassSelectorCB->clear();
	ui.AttributeList->clear();

	if (m_scene)
		onSceneDetached(m_scene);

	m_scene = scene;

	setEnabled(m_scene);

	if (m_scene)
		onSceneAttached(m_scene);
}

void CAttributeEditor::onSceneAttached(CEditorScene* scene)
{
	connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()), Qt::QueuedConnection);

	onSceneChanged();
}

void CAttributeEditor::onSceneDetached(CEditorScene* scene)
{
	scene->disconnect(this);
}

void CAttributeEditor::onSceneChanged()
{
	ui.AttributeList->setUpdatesEnabled(false);
	ui.AttributeList->blockSignals(true);

	ui.ClassSelectorCB->clear();
	ui.AttributeList->clear();

	QByteArrayList classIds = m_scene->getClasses();

	for (auto classId : classIds)
	{
		auto classTreeItem = new QTreeWidgetItem();
		classTreeItem->setText(0, QString(classId));

		ui.AttributeList->addTopLevelItem(classTreeItem);

		fillClassAttr(classId, classTreeItem);
	}

	ui.AttributeList->setUpdatesEnabled(true);
	ui.AttributeList->blockSignals(false);
}

void CAttributeEditor::fillClassAttr(const QByteArray& classId, QTreeWidgetItem* classTreeItem)
{
	AttributesMap attrs = m_scene->getClassAttributes(classId, false);
    QSet<QByteArray> visIds = m_scene->getVisibleClassAttributes(classId, false);

	for (auto attr : attrs)
	{
		auto attrItem = new QTreeWidgetItem(classTreeItem);
		attrItem->setText(0, QString(attr.id));
        attrItem->setCheckState(0, visIds.contains(attr.id) ? Qt::Checked : Qt::Unchecked);
		attrItem->setText(1, attr.name);
		attrItem->setText(2, attr.defaultValue.toString());

		attrItem->setData(0, Qt::UserRole + 1, attr.id);
		attrItem->setData(0, Qt::UserRole + 2, classId);
	}

	classTreeItem->setExpanded(true);
}

void CAttributeEditor::on_AttributeList_itemChanged(QTreeWidgetItem *item, int column)
{
	if (column == 0)
	{
		bool isVisible = (item->checkState(0) == Qt::Checked);
		QByteArray attrId = item->data(0, Qt::UserRole + 1).toByteArray();
		QByteArray classId = item->data(0, Qt::UserRole + 2).toByteArray();

		m_scene->setClassAttributeVisible(classId, attrId, isVisible);

		m_scene->addUndoState();
	}
}


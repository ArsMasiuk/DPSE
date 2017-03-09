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
	connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()));

	onSceneChanged();
}

void CAttributeEditor::onSceneDetached(CEditorScene* scene)
{
	scene->disconnect(this);
}

void CAttributeEditor::onSceneChanged()
{
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
}

void CAttributeEditor::fillClassAttr(const QByteArray& classId, QTreeWidgetItem* classTreeItem)
{
	AttributesMap attrs = m_scene->getClassAttributes(classId);

	for (auto attr : attrs)
	{
		auto attrItem = new QTreeWidgetItem(classTreeItem);
		attrItem->setText(0, QString(attr.id));
		attrItem->setCheckState(0, attr.isVisible ? Qt::Checked : Qt::Unchecked);
		attrItem->setText(1, attr.name);
		attrItem->setText(2, attr.defaultValue.toString());
	}

	classTreeItem->setExpanded(true);
}

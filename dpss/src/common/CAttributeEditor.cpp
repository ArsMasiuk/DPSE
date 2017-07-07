/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CAttributeEditor.h"
#include "CItemAttributeEditor.h"
#include "CNewAttributeDialog.h"

#include <qvge/CEditorScene.h>
#include <qvge/Utils.h>

#include <QMessageBox>


static int ClassRole = Qt::UserRole + 1;
static int AttrRole = Qt::UserRole + 2;


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

	ui.AttributeList->clear();

	QByteArrayList classIds = m_scene->getClasses();

	for (auto classId : classIds)
	{
		auto classTreeItem = new QTreeWidgetItem();
		classTreeItem->setText(0, QString(classId));
        classTreeItem->setData(0, ClassRole, classId);

		ui.AttributeList->addTopLevelItem(classTreeItem);

		fillClassAttr(classId, classTreeItem);
	}

	ui.AttributeList->setUpdatesEnabled(true);
	ui.AttributeList->blockSignals(false);
}


void CAttributeEditor::fillClassAttr(const QByteArray& classId, QTreeWidgetItem* classTreeItem)
{
	AttributesMap attrs = m_scene->getClassAttributes(classId, true);
    QSet<QByteArray> visIds = m_scene->getVisibleClassAttributes(classId, false);

	for (auto attr : attrs)
	{
		auto attrItem = new QTreeWidgetItem(classTreeItem);
		attrItem->setText(0, QString(attr.id));
        attrItem->setCheckState(0, visIds.contains(attr.id) ? Qt::Checked : Qt::Unchecked);
		attrItem->setText(1, attr.name);
		attrItem->setText(2, Utils::variantToText(attr.defaultValue));

		attrItem->setData(0, AttrRole, attr.id);
		attrItem->setData(0, ClassRole, classId);
	}

	classTreeItem->setExpanded(true);
}


void CAttributeEditor::on_AttributeList_itemChanged(QTreeWidgetItem *item, int column)
{
	if (column == 0)
	{
		bool isVisible = (item->checkState(0) == Qt::Checked);
		QByteArray attrId = item->data(0, AttrRole).toByteArray();
		QByteArray classId = item->data(0, ClassRole).toByteArray();

		m_scene->setClassAttributeVisible(classId, attrId, isVisible);

		m_scene->addUndoState();
	}
}


void CAttributeEditor::on_AttributeList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (!item)
		return;

	QByteArray attrId = item->data(0, AttrRole).toByteArray();
	QByteArray classId = item->data(0, ClassRole).toByteArray();
	if (classId.isEmpty() || attrId.isEmpty())
		return;

	QVariant attrValue = m_scene->getClassAttribute(classId, attrId);
	QVariant newValue = CItemAttributeEditor::editValue(m_scene, classId, attrId, attrValue);
	if (newValue.isNull())
		return;

	m_scene->setClassAttribute(classId, attrId, newValue);

	m_scene->addUndoState();
}


void CAttributeEditor::on_AddButton_clicked()
{
    CNewAttributeDialog attrDialog;
    int r = attrDialog.exec();
    if (r == QDialog::Rejected)
        return;

    QByteArray attrId = attrDialog.getAttrId();
    if (attrId.isEmpty())
        return;

    QString name = attrDialog.getAttrName();
    if (name.isEmpty())
        name = attrId;

    QVariant v = attrDialog.getDefaultValue();

    QByteArray classId;
    QList<QTreeWidgetItem*> selItems = ui.AttributeList->selectedItems();
    if (!selItems.isEmpty())
        classId = selItems.first()->data(0, ClassRole).toByteArray();

    CAttribute attr(attrId, name, v);
    m_scene->setClassAttribute(classId, attr, false);

    m_scene->addUndoState();
}


void CAttributeEditor::on_RemoveButton_clicked()
{
    QList<QTreeWidgetItem*> selItems = ui.AttributeList->selectedItems();
	if (selItems.isEmpty())
		return;

	auto *item = selItems.first();
	auto attrId = item->data(0, AttrRole).toByteArray();
	if (attrId.isEmpty())
		return;

	auto classId = item->data(0, ClassRole).toByteArray();

	int r = QMessageBox::question(NULL,
		tr("Remove attribute"),
		tr("Remove attribute '%1' from class '%2' attributes?").arg(QString(attrId), QString(classId)),
		QMessageBox::Yes, QMessageBox::Cancel);
	if (r == QMessageBox::Cancel)
		return;

	m_scene->removeClassAttribute(classId, attrId);

	m_scene->addUndoState();
}

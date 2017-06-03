/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include "CItemAttributeEditor.h"
#include "CExtListInputDialog.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/CNode.h>
#include <qvge/CConnection.h>

#include <QInputDialog>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>
#include <QElapsedTimer>


static int ClassRole = Qt::UserRole + 1;
static int AttrRole = Qt::UserRole + 2;


CItemAttributeEditor::CItemAttributeEditor(QWidget *parent)
	: QWidget(parent),
	m_scene(NULL)
{
	ui.setupUi(this);
}


CItemAttributeEditor::~CItemAttributeEditor()
{
}


void CItemAttributeEditor::setScene(CNodeEditorScene* scene)
{
	ui.Editor->clear();

	if (m_scene)
		onSceneDetached(m_scene);

	m_scene = scene;

	setEnabled(m_scene);

	if (m_scene)
		onSceneAttached(m_scene);
}


void CItemAttributeEditor::onSceneAttached(CEditorScene* scene)
{
	connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()), Qt::QueuedConnection);
	connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()), Qt::QueuedConnection);
}


void CItemAttributeEditor::onSceneDetached(CEditorScene* scene)
{
	scene->disconnect(this);
}


void CItemAttributeEditor::onSceneChanged()
{
	// test
	onSelectionChanged();
}


void CItemAttributeEditor::onSelectionChanged()
{
	ui.Editor->clear();

	QElapsedTimer tm;
	tm.start();

	QList<CItem*> nodes = m_scene->getSelectedItems<CNode, CItem>();
	QList<CItem*> edges = m_scene->getSelectedItems<CConnection, CItem>();

	auto *inodes = new QTreeWidgetItem();
	inodes->setText(0, tr("Nodes: %1").arg(nodes.count()));
	ui.Editor->addTopLevelItem(inodes);

	auto *iedges = new QTreeWidgetItem();
	iedges->setText(0, tr("Edges: %1").arg(edges.count()));
	ui.Editor->addTopLevelItem(iedges);

	listAttributes(inodes, nodes, "node");
    listAttributes(iedges, edges, "edge");

	qDebug() << "CItemAttributeEditor::onSelectionChanged(): " << tm.elapsed();
}


void CItemAttributeEditor::listAttributes(QTreeWidgetItem* rootItem, const QList<CItem*>& sceneItems, const QByteArray& classId)
{
	QSet<QByteArray> ids;
	QMap<QByteArray, QVariant> attrs;

	// merge ids
	for (auto sceneItem : sceneItems)
	{
		auto localAttrs = sceneItem->getLocalAttributes();
		ids.unite(localAttrs.keys().toSet());
	}

	// merge class ids
	const AttributesMap& classAttrs = m_scene->getClassAttributes(classId, true);
	ids.unite(classAttrs.keys().toSet());

	// merge attrs
	for (auto id : ids)
	{
		for (auto sceneItem : sceneItems)
		{
			if (attrs.contains(id))
			{
				if (!attrs[id].isNull())
				{
					if (attrs[id] != sceneItem->getAttribute(id))
						attrs[id] = QVariant();
				}
				else
					break;	// already null - no sense to check
			}
			else
				attrs[id] = sceneItem->getAttribute(id);
		}
	}

	for (auto it = attrs.constBegin(); it != attrs.constEnd(); ++it)
	{
		auto *item = new QTreeWidgetItem(rootItem);
		item->setText(0, it.key());
		item->setText(1, it.value().isNull() ? "<...>" : Utils::variantToText(it.value()));

        item->setData(0, ClassRole, classId);
        item->setData(0, AttrRole, it.key());
    }

	rootItem->setExpanded(true);
}


// edit attrs

QVariant CItemAttributeEditor::editValue(const QByteArray& classId, const QByteArray& attrId, const QVariant& attrValue)
{
	CAttribute attr = m_scene->getClassAttributeInfo(classId, attrId);

	// color editor
	if (attr.valueType == QVariant::Color)
	{
		QColor color = QColorDialog::getColor(attrValue.value<QColor>());
		if (!color.isValid())
			return QVariant();
		else
			return color;
	}

	// int editor
	if (attr.valueType == QVariant::Int || attr.valueType == QVariant::LongLong || attr.valueType == QVariant::UInt || attr.valueType == QVariant::ULongLong)
	{
		bool ok = true;
		int val = QInputDialog::getInt(NULL, tr("Set Attribute"), attrId, attrValue.toInt(), INT_MIN, INT_MAX, 1, &ok);
		if (!ok)
			return QVariant();
		else
			return val;
	}

	// double editor
	if (attr.valueType == QVariant::Double)
	{
		bool ok = true;
        double val = QInputDialog::getDouble(NULL,
                                             tr("Set Attribute"), attrId, attrValue.toDouble(),
                                             std::numeric_limits<double>::min(),
                                             std::numeric_limits<double>::max(),
                                             4, &ok);
		if (!ok)
			return QVariant();
		else
			return val;
	}

    // predefined attrs
	auto constrains = CAttributeConstrains::getClassConstrains(classId, attrId);
	if (constrains)
	{
		auto constrainsList = dynamic_cast<CAttributeConstrainsList*>(constrains);
		if (constrainsList)
		{
			int index = constrainsList->ids.indexOf(attrValue.toString());
			int newIndex = CExtListInputDialog::getItemIndex(
				tr("Set Attribute"), attr.name, 
				constrainsList->names, constrainsList->icons, 
				index);
			
			if (newIndex >= 0)
				return constrainsList->ids.at(newIndex);
			else
				return QVariant();
		}
	}

	// default: edit as string
	QString val = QInputDialog::getText(NULL, tr("Set Attribute"), attrId, QLineEdit::Normal, attrValue.toString());
	if (val.isEmpty())
		return QVariant();
	else
		return val;
}


void CItemAttributeEditor::on_Editor_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	auto classId = item->data(0, ClassRole).toByteArray();
	auto attrV = item->data(0, AttrRole);
    if (attrV.isNull())
        return;

    QByteArray attrId = attrV.toByteArray();

	QVariant v = editValue(classId, attrId, item->text(1));
	if (!v.isValid())
		return;

	bool isNode = (classId == "node");
	bool isEdge = (classId == "edge");

	QList<CItem*> selectedItems = 
		isNode ? m_scene->getSelectedItems<CNode, CItem>() : 
		isEdge ? m_scene->getSelectedItems<CConnection, CItem>() :
				 m_scene->getSelectedItems<CItem>();

	if (attrId == "id")
	{
		if (selectedItems.size() > 1)
		{
			int r = QMessageBox::warning(NULL, tr("Changing ID"),
				tr("More than one item selected, ID will be set to the 1st one. Continue?"),
				QMessageBox::Ok | QMessageBox::Cancel);

			if (r == QMessageBox::Cancel)
				return;
		}

		QString val = v.toString();

		if (isNode)
		{
			auto nodes = m_scene->getItemsById<CNode>(val);
			if (nodes.count()) 
			{
				int r = QMessageBox::warning(NULL, tr("Changing ID"),
					tr("Such ID exists already. Existing ID will be marked with _old if continued."),
					QMessageBox::Ok | QMessageBox::Cancel);

				if (r == QMessageBox::Cancel)
					return;

				nodes.first()->setId(val + "_old");
			}
		}
		else
		if (isEdge)
		{
			auto edges = m_scene->getItemsById<CConnection>(val);
			if (edges.count())
			{
				int r = QMessageBox::warning(NULL, tr("Changing ID"),
					tr("Such ID exists already. Existing ID will be marked with _old if continued."),
					QMessageBox::Ok | QMessageBox::Cancel);

				if (r == QMessageBox::Cancel)
					return;

				edges.first()->setId(val + "_old");
			}
		}

		// some other class...
		selectedItems.first()->setId(val);
	}
	else // other attr
		for (auto sceneItem : selectedItems)
		{
			sceneItem->setAttribute(attrId, v);
		}

    // store state
    m_scene->addUndoState();

    // rebuild tree
    onSelectionChanged();
}


void CItemAttributeEditor::on_AddButton_clicked()
{

}


void CItemAttributeEditor::on_RemoveButton_clicked()
{
	QList<QTreeWidgetItem*> selItems = ui.Editor->selectedItems();
	if (selItems.isEmpty())
		return;

	auto *item = selItems.first();
	auto attrId = item->data(0, AttrRole).toByteArray();
	if (attrId.isEmpty())
		return;

	int r = QMessageBox::question(NULL, 
		tr("Remove attribute"), 
		tr("Remove attribute %1 from selected items?").arg(QString(attrId)), 
		QMessageBox::Yes, QMessageBox::Cancel);
	if (r == QMessageBox::Cancel)
		return;

	QList<CItem*> sceneItems;
	auto classId = item->data(0, ClassRole).toByteArray();
	if (classId == "node")
	{
		sceneItems = m_scene->getSelectedItems<CNode, CItem>();
	}
	else if (classId == "edge")
	{
		sceneItems = m_scene->getSelectedItems<CConnection, CItem>();
	}

	for (auto sceneItem : sceneItems)
	{
		sceneItem->removeAttribute(attrId);
	}

	// store state
	m_scene->addUndoState();

	// rebuild tree
	onSelectionChanged();
}

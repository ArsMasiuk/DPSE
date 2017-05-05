#include "CItemAttributeEditor.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/CNode.h>
#include <qvge/CConnection.h>

#include <QInputDialog>
#include <QMessageBox>


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

void CItemAttributeEditor::on_Editor_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
    auto attrV = item->data(0, AttrRole);
    if (attrV.isNull())
        return;

    QByteArray attrId = attrV.toByteArray();

    QString val = QInputDialog::getText(NULL, tr("Set Attribute"), attrId, QLineEdit::Normal, item->text(1));
    if (val.isEmpty())
        return;

	QList<CItem*> sceneItems = m_scene->getSelectedItems<CItem>();

	if (attrId == "id")
	{
		if (sceneItems.size() > 1)
		{
			int r = QMessageBox::warning(NULL, tr("Changing ID"),
				tr("More than one item selected, ID will be set to the 1st one. Continue?"),
				QMessageBox::Ok | QMessageBox::Cancel);

			if (r == QMessageBox::Cancel)
				return;
		}

		auto classId = item->data(0, ClassRole).toByteArray();

		if (classId == "node")
		{
			CNode* node = m_scene->nodeById(val);
			if (node) 
			{
				int r = QMessageBox::warning(NULL, tr("Changing ID"),
					tr("Such ID exists already. Existing ID will be marked with _old if continued."),
					QMessageBox::Ok | QMessageBox::Cancel);

				if (r == QMessageBox::Cancel)
					return;

				node->setId(val + "_old");
			}
		}
		else
		if (classId == "edge")
		{
			CConnection* edge = m_scene->edgeById(val);
			if (edge)
			{
				int r = QMessageBox::warning(NULL, tr("Changing ID"),
					tr("Such ID exists already. Existing ID will be marked with _old if continued."),
					QMessageBox::Ok | QMessageBox::Cancel);

				if (r == QMessageBox::Cancel)
					return;

				edge->setId(val + "_old");
			}
		}

		// some other class...
		sceneItems.first()->setId(val);
	}
	else // other attr
		for (auto sceneItem : sceneItems)
		{
			sceneItem->setAttribute(attrId, val);
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
	QList<QTreeWidgetItem *> selItems = ui.Editor->selectedItems();
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

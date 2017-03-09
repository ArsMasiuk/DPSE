#include "CItemAttributeEditor.h"

#include <qvge/CEditorScene.h>
#include <qvge/CNode.h>
#include <qvge/CConnection.h>


CItemAttributeEditor::CItemAttributeEditor(QWidget *parent)
	: QWidget(parent),
	m_scene(NULL)
{
	ui.setupUi(this);
}

CItemAttributeEditor::~CItemAttributeEditor()
{

}

void CItemAttributeEditor::setScene(CEditorScene* scene)
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
	connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));
}

void CItemAttributeEditor::onSceneDetached(CEditorScene* scene)
{
	scene->disconnect(this);
}

void CItemAttributeEditor::onSelectionChanged()
{
	// test
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
	const AttributesMap& classAttrs = m_scene->getClassAttributes(classId);
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
	}

	rootItem->setExpanded(true);
}

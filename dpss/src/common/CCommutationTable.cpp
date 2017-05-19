/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CCommutationTable.h"

#include <qvge/CNodeEditorScene.h>
#include <qvge/CConnection.h>
#include <qvge/CNode.h>


// NumSortItem: numeric sorting by ids

class NumSortItem : public QTreeWidgetItem
{
public:
	bool operator < (const QTreeWidgetItem &other) const
	{
		int col = treeWidget()->sortColumn();
		bool b1, b2;
		int i1 = text(col).toInt(&b1);
		int i2 = other.text(col).toInt(&b2);
		if (b1 && b2)
			return i1 < i2;

		return QTreeWidgetItem::operator < (other);
	}
};


// CCommutationTable

CCommutationTable::CCommutationTable(QWidget *parent)
	: QWidget(parent),
	m_scene(NULL)
{
	ui.setupUi(this);
}

CCommutationTable::~CCommutationTable()
{

}

void CCommutationTable::setScene(CNodeEditorScene* scene)
{
	ui.Table->clear();

	if (m_scene)
		onSceneDetached(m_scene);

	m_scene = scene;

	setEnabled(m_scene);

	if (m_scene)
		onSceneAttached(m_scene);
}

void CCommutationTable::connectSignals(CEditorScene* scene)
{
	connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()), Qt::QueuedConnection);
	connect(scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()), Qt::QueuedConnection);
}

void CCommutationTable::onSceneAttached(CEditorScene* scene)
{
	connectSignals(scene);

	onSceneChanged();
}

void CCommutationTable::onSceneDetached(CEditorScene* scene)
{
	scene->disconnect(this);
}

void CCommutationTable::onSceneChanged()
{
	ui.Table->setUpdatesEnabled(false);
	ui.Table->blockSignals(true);

	ui.Table->clear();

	QList<CConnection*> edges = m_scene->getItems<CConnection>();
	for (auto edge : edges)
	{
		auto item = new NumSortItem();
		ui.Table->addTopLevelItem(item);

		item->setText(0, edge->firstNode()->getId());
		item->setText(1, edge->lastNode()->getId());
		item->setText(2, edge->getId());
	}

	ui.Table->setUpdatesEnabled(true);
	ui.Table->blockSignals(false);
}

void CCommutationTable::onSelectionChanged()
{
	ui.Table->setUpdatesEnabled(false);
	ui.Table->blockSignals(true);

	ui.Table->clearSelection();

	QList<CConnection*> edges = m_scene->getSelectedItems<CConnection>();
	for (auto edge : edges)
	{
		auto edgeId = edge->getId();
		auto foundItems = ui.Table->findItems(edgeId, Qt::MatchCaseSensitive, 2);
		for (auto item : foundItems)
		{
			item->setSelected(true);
			ui.Table->scrollToItem(item);
		}
	}

	ui.Table->setUpdatesEnabled(true);
	ui.Table->blockSignals(false);
}

void CCommutationTable::on_Table_itemSelectionChanged()
{
	if (!m_scene)
		return;

	ui.Table->blockSignals(true);
	m_scene->disconnect(this);

	m_scene->deselectAll();

	auto selTableItems = ui.Table->selectedItems();
	QSet<QString> selIds;
	for (auto item : selTableItems)
	{
		selIds.insert(item->text(2));
	}

	QList<CConnection*> edges = m_scene->getItems<CConnection>();
	for (auto edge : edges)
	{
		if (selIds.contains(edge->getId()))
		{
			edge->setSelected(true);
			edge->ensureVisible();
		}
	}

	ui.Table->blockSignals(false);
	connectSignals(m_scene);
}


void CCommutationTable::on_Table_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	if (!m_scene || !item)
		return;

	if (column < 2)
	{
		CNode* node = m_scene->nodeById(item->text(column));
		if (node)
		{
			m_scene->deselectAll();
			node->setSelected(true);
			node->ensureVisible();
			return;
		}
	}

	if (column == 2)
	{
		auto* edge = m_scene->edgeById(item->text(column));
		if (edge)
		{
			m_scene->deselectAll();
			edge->setSelected(true);
			edge->ensureVisible();
			return;
		}
	}
}

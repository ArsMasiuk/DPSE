#include "CCommutationTable.h"

#include <qvge/CEditorScene.h>
#include <qvge/CConnection.h>
#include <qvge/CNode.h>


CCommutationTable::CCommutationTable(QWidget *parent)
	: QWidget(parent),
	m_scene(NULL)
{
	ui.setupUi(this);
}

CCommutationTable::~CCommutationTable()
{

}

void CCommutationTable::setScene(CEditorScene* scene)
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
		auto item = new QTreeWidgetItem();
		ui.Table->addTopLevelItem(item);

		item->setText(0, edge->firstNode()->getAttribute("id").toString());
		item->setText(1, edge->lastNode()->getAttribute("id").toString());
		item->setText(2, edge->getAttribute("id").toString());
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
		auto edgeId = edge->getAttribute("id").toString();
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

	//m_scene->deselectAll();
	//auto selItems = m_scene->selectedItems();
	//for (auto item : selItems)
	//	item->setSelected(false);

	auto selTableItems = ui.Table->selectedItems();
	QSet<QString> selIds;
	for (auto item : selTableItems)
	{
		selIds.insert(item->text(2));
	}

	QList<CConnection*> edges = m_scene->getItems<CConnection>();
	for (auto edge : edges)
	{
		edge->setSelected(selIds.contains(edge->getAttribute("id").toString()));
	}

	ui.Table->blockSignals(false);
	connectSignals(m_scene);
}

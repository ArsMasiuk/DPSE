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

void CCommutationTable::onSceneAttached(CEditorScene* scene)
{
	connect(scene, SIGNAL(sceneChanged()), this, SLOT(onSceneChanged()), Qt::QueuedConnection);

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

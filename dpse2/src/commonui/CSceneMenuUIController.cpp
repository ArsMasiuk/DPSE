#include "CSceneMenuUIController.h"

#include <QMenu>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CNode.h>
#include <qvge/CEdge.h>


CSceneMenuUIController::CSceneMenuUIController(QObject *parent) : QObject(parent)
{
}


CSceneMenuUIController::~CSceneMenuUIController()
{
}


bool CSceneMenuUIController::exec(CEditorScene * scene, QGraphicsItem * triggerItem, QGraphicsSceneContextMenuEvent * contextMenuEvent)
{
	QMenu menu;

	// add default actions
	QAction *deleteAction = menu.addAction(tr("Delete"), scene, SLOT(onActionDelete()));
	deleteAction->setEnabled(scene->createSelectedList(CDeletableItems()).size());

	// add default node actions
	menu.addSeparator();

	int nodesCount = scene->getSelectedItems<CNode>(true).size();
	bool nodesSelected = (nodesCount > 0);

	QAction *linkAction = menu.addAction(tr("Link"), scene, SLOT(onActionLink()));
	linkAction->setEnabled(nodesCount > 1);

	QAction *unlinkAction = menu.addAction(tr("Unlink"), scene, SLOT(onActionUnlink()));
	unlinkAction->setEnabled(nodesSelected);

	QAction *nodeColorAction = menu.addAction(tr("Node(s) Color..."), scene, SLOT(onActionNodeColor()));
	nodeColorAction->setEnabled(nodesSelected);

	QAction *factorAction = menu.addAction(tr("Factor Nodes..."), parent(), SLOT(factorNodes()));
	factorAction->setEnabled(nodesCount > 1);

	menu.addSeparator();

	QAction *addPortAction = menu.addAction(tr("Add Port..."), scene, SLOT(onActionAddPort()));
	addPortAction->setEnabled(nodesCount == 1);

	QAction *editPortAction = menu.addAction(tr("Edit Port..."), scene, SLOT(onActionEditPort()));
	editPortAction->setEnabled(dynamic_cast<CNodePort*>(triggerItem));

	// add default edge actions
	menu.addSeparator();

	bool edgesSelected = scene->getSelectedItems<CEdge>(true).size();

	QAction *edgeColorAction = menu.addAction(tr("Edge(s) Color..."), scene, SLOT(onActionEdgeColor()));
	edgeColorAction->setEnabled(edgesSelected);

	QMenu *arrowsMenu = menu.addMenu(tr("Direction"));
	arrowsMenu->setEnabled(edgesSelected);
	arrowsMenu->addAction(tr("Directed"), scene, SLOT(onActionEdgeDirected()));
	arrowsMenu->addAction(tr("Mutual"), scene, SLOT(onActionEdgeMutual()));
	arrowsMenu->addAction(tr("None"), scene, SLOT(onActionEdgeUndirected()));
	arrowsMenu->addSeparator();
	arrowsMenu->addAction(tr("Reverse"), scene, SLOT(onActionEdgeReverse()));


	// execute
	menu.exec(contextMenuEvent->screenPos());

	return false;
}

#include "CSceneMenuUIController.h"

#include <QMenu>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>

#include <qvge/CNodeEditorScene.h>
#include <qvge/CNodeSceneActions.h>
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
	auto sceneActions = scene->getActions();

	QMenu menu;

	// add default actions
	QAction *deleteAction = menu.addAction(tr("Delete"), scene, SLOT(onActionDelete()));
	deleteAction->setEnabled(scene->createSelectedList(CDeletableItems()).size());

	// add default node actions
	menu.addSeparator();

	int nodesCount = scene->getSelectedItems<CNode>(true).size();
	bool nodesSelected = (nodesCount > 0);

	QAction *linkAction = menu.addAction(tr("Link"), sceneActions, SLOT(onActionLink()));
	linkAction->setEnabled(nodesCount > 1);

	QAction *unlinkAction = menu.addAction(tr("Unlink"), sceneActions, SLOT(onActionUnlink()));
	unlinkAction->setEnabled(nodesSelected);

	QAction *nodeColorAction = menu.addAction(tr("Node(s) Color..."), sceneActions, SLOT(onActionNodeColor()));
	nodeColorAction->setEnabled(nodesSelected);

	QAction *factorAction = menu.addAction(tr("Factor Nodes..."), parent(), SLOT(factorNodes()));
	factorAction->setEnabled(nodesCount > 1);

	menu.addSeparator();

	QAction *addPortAction = menu.addAction(tr("Add Port..."), parent(), SLOT(addNodePort()));
	addPortAction->setEnabled(nodesCount == 1);

	QAction *editPortAction = menu.addAction(tr("Edit Port..."), parent(), SLOT(editNodePort()));
	editPortAction->setEnabled(dynamic_cast<CNodePort*>(triggerItem));

	// add default edge actions
	menu.addSeparator();

	bool edgesSelected = scene->getSelectedItems<CEdge>(true).size();

	QAction *edgeColorAction = menu.addAction(tr("Edge(s) Color..."), sceneActions, SLOT(onActionEdgeColor()));
	edgeColorAction->setEnabled(edgesSelected);

	QMenu *arrowsMenu = menu.addMenu(tr("Direction"));
	arrowsMenu->setEnabled(edgesSelected);
	arrowsMenu->addAction(tr("Directed"), sceneActions, SLOT(onActionEdgeDirected()));
	arrowsMenu->addAction(tr("Mutual"), sceneActions, SLOT(onActionEdgeMutual()));
	arrowsMenu->addAction(tr("None"), sceneActions, SLOT(onActionEdgeUndirected()));
	arrowsMenu->addSeparator();
	arrowsMenu->addAction(tr("Reverse"), sceneActions, SLOT(onActionEdgeReverse()));


	// execute
	menu.exec(contextMenuEvent->screenPos());

	return false;
}

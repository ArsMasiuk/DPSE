#include "CBranchEditorScene.h"
#include "CNode.h"
#include "CConnection.h"

#include <QGraphicsSceneMouseEvent>


/// CBranchEditorScene

CBranchEditorScene::CBranchEditorScene(QObject *parent)
	: CEditorScene(parent),
	m_menuNode(NULL)
{
	setSelectionMode();

	m_unlinkAction = m_nodeMenu.addAction(tr("Unlink"), this, SLOT(doUnlinkNode()));
	m_nodeMenu.addSeparator();
	m_nodeMenu.addAction(tr("Delete"), this, SLOT(doDeleteNode()));
}

CBranchEditorScene::~CBranchEditorScene()
{

}


CItem* CBranchEditorScene::createItemOfType(const QByteArray & id) const
{
	if (id == "CRectNode")
		return new CRectNode();
	
	if (id == "CRoundNode")
		return new CRoundNode();
	 
	if (id == "CConnection")
		return new CConnection();

	return NULL;
}


void CBranchEditorScene::setSelectionMode()
{
	m_mode = EM_SELECT;

	//m_nodeDragController.setCreateMode(false);

	setSceneController(&m_nodeDragController);
}


void CBranchEditorScene::setBranchesMode()
{
	m_mode = EM_ADD_BRANCH;

	//m_nodeDragController.setCreateMode(true);

	setSceneController(&m_nodeDragController);
}

void CBranchEditorScene::setFansMode()
{
	m_mode = EM_ADD_FAN;

	setSceneController(NULL);
}

void CBranchEditorScene::doUnlinkNode()
{
	m_menuNode->unlink();
}

void CBranchEditorScene::doDeleteNode()
{
	delete m_menuNode;
}


void CBranchEditorScene::onItemPressedRight(CItem * citem, QGraphicsSceneMouseEvent * mouseEvent)
{
	m_menuNode = dynamic_cast<CNode*>(citem);
	if (!m_menuNode)
		return;

	m_unlinkAction->setEnabled(m_menuNode && m_menuNode->getConnections().count() > 1);

	m_nodeMenu.popup(mouseEvent->screenPos());
}


// privates

CBranchNode::CBranchNode(QGraphicsItem * parent) : Super(parent)
{
	setBrush(Qt::cyan);
}

#include "CNodeDragController.h"
#include "CNode.h"
#include "CConnection.h"

#include <QGraphicsSceneMouseEvent>


CNodeDragController::CNodeDragController():
	m_tempStartNode(NULL),
	m_tempEndNode(NULL),
	m_tempConnection(NULL),
	m_tempStartReal(false),
	m_createMode(false)
{
}

CNodeDragController::~CNodeDragController()
{
}

void CNodeDragController::setCreateMode(bool on)
{
	if (m_createMode != on)
	{
		doCleanup();

		m_createMode = on;
	}
}

CNode* CNodeDragController::createNode(CNode * pressedNode)
{
	return pressedNode ? dynamic_cast<CNode*>(pressedNode->clone()) : new CRoundNode();
}


// reimp

void CNodeDragController::reset()
{
	doCleanup();

	Super::reset();
}

CItem* CNodeDragController::acceptDragStarted(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	// check if it is node item
	CNode* node = dynamic_cast<CNode*>(pressedItem);
	if (node && m_createMode)
	{
		m_tempStartNode = node;
		m_tempStartReal = true;

		m_tempEndNode = createNode(node);
		Q_ASSERT(m_tempEndNode);
		
		scene->addItem(dynamic_cast<QGraphicsItem*>(m_tempEndNode));
		m_tempEndNode->moveTo(mouseEvent->scenePos());

		// Ctrl starts clone connection instead (to revise!)
		if (mouseEvent->modifiers() & Qt::ControlModifier)
		{
			QSet<CConnection*> conns = m_tempStartNode->getConnections();
			foreach (CConnection* c, conns)
			{
				c = c->clone();
				c->reattach(node, m_tempEndNode);
				scene->addItem(c);
			}
		}
		else // normal connection
		{
			m_tempConnection = new CConnection();
			scene->addItem(m_tempConnection);

			m_tempConnection->setFirstNode(node);
			m_tempConnection->setLastNode(m_tempEndNode);
		}

		// end node is node to drag
		return m_tempEndNode;
	}

	// propagate drag to super
	return Super::acceptDragStarted(scene, pressedItem, hoverItem, mouseEvent);
}


void CNodeDragController::dragFinished(CEditorScene * scene, CItem * draggedItem, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	CNode *dragNode = dynamic_cast<CNode*>(draggedItem);
	if (!dragNode)
		return;

	// check start node for collisions (because it could be moved by grid snap later)
	if (m_tempStartNode && !m_tempStartReal)
	{
		QList<CNode*> collidingNodes = m_tempStartNode->getCollidingNodes();

		for (int i = 0; i < collidingNodes.size(); ++i)
		{
			CNode *node = collidingNodes.at(i);

			// check if it is end node
			if (node == m_tempEndNode)
			{
				doCleanup();
				return;
			}

			// if not: merge with it
			m_tempStartReal = true;
			m_tempStartNode->merge(node);
		}
	}

	// check end node for collisions
	QList<CNode*> collidingNodes = dragNode->getCollidingNodes();

	for (int i = 0; i < collidingNodes.size(); ++i)
	{
		CNode *node = collidingNodes.at(i);

		// check if it is start node
		if (m_tempStartNode == node)
		{
			doCleanup();
			return;
		}

		// merge this node with dragged one
		onItemDropped(scene, draggedItem, node);

		node->merge(dragNode);

		// drop temps if any
		m_tempStartNode = m_tempEndNode = NULL;
		m_tempConnection = NULL;

		return;
	}

	// else drop on scene
	if (m_tempEndNode)
	{
		onItemDropped(scene, draggedItem, NULL);

		m_tempStartNode = m_tempEndNode = NULL;
		m_tempConnection = NULL;
	}
}


bool CNodeDragController::acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	// item pressed
	if (pressedItem/* || !m_createMode*/)
		return Super::acceptMousePressed(scene, pressedItem, mouseEvent);

	// no pressed item -> create temp nodes
	return startNewDrag(scene, NULL, mouseEvent);
}


// protected

bool CNodeDragController::startNewDrag(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	CNode * pressedNode = dynamic_cast<CNode*>(pressedItem);

	if (pressedNode)
	{
		// start drag from node
		m_tempStartReal = true;
		m_tempStartNode = pressedNode;
	}
	else
	{
		// scene pressed
		m_tempStartReal = false;

		m_tempStartNode = createNode();
		scene->addItem(dynamic_cast<QGraphicsItem*>(m_tempStartNode));
		m_tempStartNode->moveTo(mouseEvent->scenePos());
	}

	m_tempEndNode = createNode();
	scene->addItem(dynamic_cast<QGraphicsItem*>(m_tempEndNode));
	m_tempEndNode->moveTo(mouseEvent->scenePos());

	m_tempConnection = new CConnection();
	scene->addItem(m_tempConnection);

	m_tempConnection->setFirstNode(m_tempStartNode);
	m_tempConnection->setLastNode(m_tempEndNode);

	setDragItem(m_tempEndNode);

	return true;
}


// private

void CNodeDragController::doCleanup()
{
	if (m_tempConnection)
	{
		delete m_tempConnection;
		m_tempConnection = NULL;

		// connected nodes will be killed automatically here...
		m_tempStartNode = NULL;
		m_tempEndNode = NULL;
	}

	//if (m_tempStartNode)
	//{
	//	if (!m_tempStartReal)
	//		delete m_tempStartNode;

	//	m_tempStartNode = NULL;
	//}

	//if (m_tempEndNode)
	//{
	//	delete m_tempEndNode;
	//	m_tempEndNode = NULL;
	//}

	m_tempStartReal = false;
}


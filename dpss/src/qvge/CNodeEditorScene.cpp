#include "CNodeEditorScene.h"
#include "CNode.h"
#include "CConnection.h"

#include <QGraphicsSceneMouseEvent>
#include <QColorDialog> 
#include <QKeyEvent>
#include <QDebug>


CNodeEditorScene::CNodeEditorScene(QObject * parent) : Super(parent),
	m_startNode(NULL),
	m_endNode(NULL),
	m_connection(NULL),
	m_realStart(false),
	m_activeConnectionFactory(NULL),
	m_state(IS_None)
{
	// default factories
	registerItemFactory<CConnection>();
	registerItemFactory<CNode>();
}

// reimp

void CNodeEditorScene::initialize()
{
	Super::initialize();

	// default node attr
    CAttribute nodeAttr("color", "Color", QColor(Qt::magenta));
	setClassAttribute("node", nodeAttr);

    CAttribute shapeAttr("shape", "Shape", "disc");
	setClassAttribute("node", shapeAttr);

	//CAttribute sizeAttr("size", "Size", 11.0);
	//setClassAttribute("node", sizeAttr);
	//setClassAttributeConstrains("node", "size", new CDoubleConstrains(0.1, 1000.0));
	createClassAttribute("node", "size", "Size", 11.0, new CDoubleConstrains(0.1, 1000.0));

    CAttribute posAttr("pos", "Position", QPointF());
    setClassAttribute("node", posAttr, true);

	// default edge attr
    CAttribute edgeAttr("color", "Color", QColor(Qt::gray));
	setClassAttribute("edge", edgeAttr);

	CAttribute directionAttr("direction", "Direction", "directed");
	setClassAttribute("edge", directionAttr);

	CAttribute weightAttr("weight", "Weight", 1.0);
	setClassAttribute("edge", weightAttr);


	CAttributeConstrainsList *edgeDirections = new CAttributeConstrainsList();
	edgeDirections->names << "Directed (one end)" << "Mutual (both ends)" << "None (no ends)";
	edgeDirections->ids << "directed" << "mutual" << "undirected";
	edgeDirections->icons << QIcon(":/Icons/Edge-Directed") << QIcon(":/Icons/Edge-Mutual") << QIcon(":/Icons/Edge-Undirected");
	setClassAttributeConstrains("edge", "direction", edgeDirections);

	CAttributeConstrainsList *nodeShapes = new CAttributeConstrainsList();
	nodeShapes->names << "Dics" << "Square" << "Triangle" << "Diamond";
	nodeShapes->ids << "disc" << "square" << "triangle" << "diamond";
	nodeShapes->icons << QIcon(":/Icons/Node-Disc") << QIcon(":/Icons/Node-Square") << QIcon(":/Icons/Node-Triangle") << QIcon(":/Icons/Node-Diamond");
	setClassAttributeConstrains("node", "shape", nodeShapes);
}


void CNodeEditorScene::initializeOnce()
{
	Super::initializeOnce();
}


// menu slots

void CNodeEditorScene::onActionUnlink()
{
	QList<CNode*> nodes = getSelectedItems<CNode>(true);
	if (nodes.isEmpty())
		return;

	for (auto node : nodes)
	{
		node->unlink();
	}

	addUndoState();
}

void CNodeEditorScene::onActionNodeColor()
{
	QList<CNode*> nodes = getSelectedItems<CNode>(true);
	if (nodes.isEmpty())
		return;

	QColor color = QColorDialog::getColor(nodes.first()->getAttribute("color").value<QColor>());
	if (!color.isValid())
		return;

	for (auto node: nodes)
	{
		node->setAttribute("color", color);
	}

	addUndoState();
}

void CNodeEditorScene::onActionEdgeColor()
{
	QList<CConnection*> edges = getSelectedItems<CConnection>(true);
	if (edges.isEmpty())
		return;

	QColor color = QColorDialog::getColor(edges.first()->getAttribute("color").value<QColor>());
	if (!color.isValid())
		return;

	for (auto edge : edges)
	{
		edge->setAttribute("color", color);
	}

	addUndoState();
}

void CNodeEditorScene::onActionEdgeReverse()
{
	QList<CConnection*> edges = getSelectedItems<CConnection>(true);
	if (edges.isEmpty())
		return;

	for (auto edge : edges)
	{
		edge->reverse();
	}

	addUndoState();
}

void CNodeEditorScene::onActionEdgeDirected()
{
	QList<CConnection*> edges = getSelectedItems<CConnection>(true);
	if (edges.isEmpty())
		return;

	for (auto edge : edges)
	{
		edge->setAttribute("direction", "directed");
		edge->update();
	}

	addUndoState();
}

void CNodeEditorScene::onActionEdgeMutual()
{
	QList<CConnection*> edges = getSelectedItems<CConnection>(true);
	if (edges.isEmpty())
		return;

	for (auto edge : edges)
	{
		edge->setAttribute("direction", "mutual");
		edge->update();
	}

	addUndoState();
}

void CNodeEditorScene::onActionEdgeUndirected()
{
	QList<CConnection*> edges = getSelectedItems<CConnection>(true);
	if (edges.isEmpty())
		return;

	for (auto edge : edges)
	{
		edge->setAttribute("direction", "undirected");
		edge->update();
	}

	addUndoState();
}

// actions

bool CNodeEditorScene::startNewConnection(const QPointF& pos)
{
	QGraphicsItem* item = itemAt(pos, QTransform());
	if (item)
	{
		if (!item->isEnabled())
			return false;

		CNode *node = dynamic_cast<CNode*>(item);
		if (!node)
			return false;

		if (!node->allowStartConnection())
			return false;

		m_realStart = false;
		m_startNode = node;
	}
	else
	{
		m_realStart = true;
		m_startNode = createNewNode();
		item = m_startNode;
		addItem(item);
		item->setPos(pos);
	}

	m_endNode = dynamic_cast<CNode*>(m_startNode->clone());

	m_connection = createNewConnection();
	addItem(m_connection);
	m_connection->setFirstNode(m_startNode);
	m_connection->setLastNode(m_endNode);

	m_state = IS_Creating;

	return true;
}

void CNodeEditorScene::cancel(const QPointF& /*pos*/)
{
	m_state = IS_None;

	if (!m_endNode)
		return;

	// cancel current drag operation
	Super::finishDrag(NULL, m_endNode, true);


	// kill connector
	m_connection->setFirstNode(NULL);
	m_connection->setLastNode(NULL);
	delete m_connection;
	m_connection = NULL;

	// kill end
	delete m_endNode;
	m_endNode = NULL;

	// kill start if real
	if (m_realStart)
		delete m_startNode;

	m_startNode = NULL;
	m_realStart = false;
}

CNode* CNodeEditorScene::createNewNode() const
{
	if (getActiveItemFactory()) 
	{
		CItem* nodeItem = getActiveItemFactory()->create();
		if (nodeItem) 
		{
			if (CNode* node = dynamic_cast<CNode*>(nodeItem))
				return node;

			delete nodeItem;
		}
	}

	// here default
	return new CNode();
}

CConnection* CNodeEditorScene::createNewConnection() const
{
	if (m_activeConnectionFactory)
	{
		CItem* connItem = m_activeConnectionFactory->create();
		if (connItem) 
		{
			if (CConnection* conn = dynamic_cast<CConnection*>(connItem))
				return conn;

			delete connItem;
		}
	}

	// here default
	return new CConnection();
}

CConnection* CNodeEditorScene::activateConnectionFactory(const QByteArray& factoryId)
{
	if (factoryId.isEmpty() || !m_itemFactories.contains(factoryId))
	{
		m_activeConnectionFactory = NULL;
	}
	else
	{
		m_activeConnectionFactory = dynamic_cast<CConnection*>(m_itemFactories[factoryId]);
	}

	return NULL;
}

// events

void CNodeEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	setSceneCursor(Qt::SizeAllCursor);

	Super::mouseDoubleClickEvent(mouseEvent);
}

void CNodeEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (m_state == IS_None)
	{
		if (!m_doubleClick)
		{
			// call super
			Super::mouseMoveEvent(mouseEvent);
			return;
		}

		// start connection at click point
		startNewConnection(m_leftClickPos);
	}

	// custom dragging
	m_endNode->setPos(mouseEvent->scenePos());

	moveDrag(mouseEvent, dynamic_cast<QGraphicsItem*>(m_endNode));
}

void CNodeEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (m_state == IS_None)
	{
		// call super
 		Super::mouseReleaseEvent(mouseEvent);
		return;
	}

	// necessary to handle scene events properly
	QGraphicsScene::mouseReleaseEvent(mouseEvent);

	// release local grabber if any
	if (m_state == IS_Creating)
	{
		m_state = IS_Finishing;

		// cancel on RMB
		if (mouseEvent->button() == Qt::RightButton)
		{
			m_state = IS_Cancelling;
		}

		// cancel on same position
		if (m_startNode->pos() == m_endNode->pos())
		{
			m_state = IS_Cancelling;
		}
	}

	// call super
	finishDrag(mouseEvent, dynamic_cast<QGraphicsItem*>(m_endNode), m_state == IS_Cancelling);

	// finish
	if (m_state == IS_Cancelling)
	{
		cancel(mouseEvent->scenePos());
	}

	m_state = IS_None;
}

void CNodeEditorScene::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->key() == Qt::Key_Escape)
	{
		cancel();
		return;
	}

	Super::keyPressEvent(keyEvent);
}

// reimp

bool CNodeEditorScene::populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems)
{
	if (!Super::populateMenu(menu, item, selectedItems))
		return false;

	// add default node actions
	menu.addSeparator();

	bool nodesSelected = getSelectedItems<CNode>(true).size();

	QAction *unlinkAction = menu.addAction(tr("Unlink"), this, SLOT(onActionUnlink()));
	unlinkAction->setEnabled(nodesSelected);

	QAction *nodeColorAction = menu.addAction(tr("Node(s) Color..."), this, SLOT(onActionNodeColor()));
	nodeColorAction->setEnabled(nodesSelected);

	// add default edge actions
	menu.addSeparator();

	bool edgesSelected = getSelectedItems<CConnection>(true).size();

	QAction *edgeColorAction = menu.addAction(tr("Connection(s) Color..."), this, SLOT(onActionEdgeColor()));
	edgeColorAction->setEnabled(edgesSelected);

	QMenu *arrowsMenu = menu.addMenu(tr("Direction"));
	arrowsMenu->setEnabled(edgesSelected);
	arrowsMenu->addAction(tr("Directed"), this, SLOT(onActionEdgeDirected()));
	arrowsMenu->addAction(tr("Mutual"), this, SLOT(onActionEdgeMutual()));
	arrowsMenu->addAction(tr("None"), this, SLOT(onActionEdgeUndirected()));
	arrowsMenu->addSeparator();
	arrowsMenu->addAction(tr("Reverse"), this, SLOT(onActionEdgeReverse()));

	return true;
}

// evaluators

bool CNodeItems::evaluate(const QGraphicsItem& item) const
{
	return (dynamic_cast<const CNode*>(&item) != NULL);
}

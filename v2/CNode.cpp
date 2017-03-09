#include "CNode.h"
#include "CConnection.h"

#include <QPen>
#include <QBrush>

////////////////////////////////////////////////////////////////////
/// \brief CNode::CNode

CNode::~CNode()
{
	foreach(CConnection *conn, m_connections)
	{
		conn->onNodeDetach(this);
	}

	// ? kill all connections as well ?
	qDeleteAll(m_connections);
}

void CNode::merge(CNode *node, bool allowCircled)
{
	if (node == this || !node)
		return;

	// make a copy because node's connections list will be updated
	QSet<CConnection*> toReconnect = node->m_connections;

	foreach(CConnection *conn, toReconnect)
	{
		conn->reattach(node, this);
	}

	if (!allowCircled)
	{
		QList<CConnection*> toDelete;

		foreach(CConnection *conn, m_connections)
		{
			if (conn->isCircled())
				toDelete.append(conn);
		}

		if (toDelete.size())
			qDeleteAll(toDelete);
	}

	// kill old node
	//delete node;	// will be removed by removed connections
}

QList<CNode*> CNode::unlink()
{
	QList<CNode*> nodes;

	float xpos = getPos().x();
	float ypos = getPos().y();

	float xstep = getScene() ? getScene()->getGridSize() : 10;

	while (m_connections.size() >= 2)
	{
		CConnection *c = *m_connections.begin();
		
		CNode *n = dynamic_cast<CNode*>(clone());
		xpos += xstep;
		n->moveTo(QPointF(xpos, ypos));

		c->reattach(this, n);

		nodes.append(n);
	}

	return nodes;
}

QList<CNode*> CNode::getCollidingNodes() const
{
	QList<CNode*> nodes;

	CEditorScene* scene = getScene();
	if (!scene)
		return nodes;

	QList<QGraphicsItem*> itemsUnderMouse = scene->items(getPos());

	for (int i = 0; i < itemsUnderMouse.size(); ++i)
	{
		CNode *node = dynamic_cast<CNode*>(itemsUnderMouse.at(i));
		if (!node || node == this)
			continue;
		
		nodes.append(node);
	}

	return nodes;
}

///
/// \brief CNode::onConnectionAttach
/// \param conn
///
void CNode::onConnectionAttach(CConnection *conn)
{
	Q_ASSERT(conn != NULL);
	//Q_ASSERT(!m_connections.contains(conn));

	m_connections.insert(conn);
}

///
/// \brief CNode::onConnectionDetach
/// \param conn
///
void CNode::onConnectionDetach(CConnection *conn)
{
	Q_ASSERT(conn != NULL);
	//Q_ASSERT(m_connections.contains(conn));

	m_connections.remove(conn);

	// ? remove orphan node ?
	if (m_connections.isEmpty())
		delete this;
}

void CNode::onItemMoved()
{
	foreach(CConnection *conn, m_connections)
	{
		conn->onNodeMoved(this);
	}
}


////////////////////////////////////////////////////////////////////
/// \brief CRectNode::CRectNode
/// \param parent

CRectNode::CRectNode(QGraphicsItem *parent) : Super(parent)
{
	setRect(-5, -5, 11, 11);

	setPen(QPen(Qt::blue));
	setBrush(Qt::green);
}


////////////////////////////////////////////////////////////////////
/// \brief CRoundNode::CRoundNode
/// \param parent

CRoundNode::CRoundNode(QGraphicsItem *parent) : Super(parent)
{
	setRect(-5, -5, 11, 11);

	setPen(QPen(Qt::blue));
	setBrush(Qt::yellow);

	setAcceptHoverEvents(true);
	setFlag(ItemIsMovable, true);
}

void CRoundNode::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setPen(QPen(Qt::blue, 2));
}

void CRoundNode::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setPen(QPen(Qt::blue));
}

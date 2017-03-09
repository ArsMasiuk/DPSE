#include "CConnection.h"

CConnection::CConnection(QGraphicsItem *parent): QGraphicsLineItem(parent)
{
    m_firstNode = m_lastNode = NULL;

    setPen(QPen(Qt::red));

    setZValue(-1);

    setFlags(QGraphicsItem::ItemIsSelectable);
}

void CConnection::setFirstNode(CNode *node)
{
    if (m_firstNode)
        m_firstNode->onConnectionDetach(this);

    m_firstNode = node;

    if (m_firstNode)
    {
        m_firstNode->onConnectionAttach(this);

        setLine(QLineF(node->getPos(), line().p2()));
    }

    update();
}

void CConnection::setLastNode(CNode *node)
{
    if (m_lastNode)
        m_lastNode->onConnectionDetach(this);

    m_lastNode = node;

    if (m_lastNode)
    {
        m_lastNode->onConnectionAttach(this);

        setLine(QLineF(line().p1(), node->getPos()));
    }

    update();
}

void CConnection::onNodeMoved(CNode *node)
{
    Q_ASSERT(node == m_firstNode || node == m_lastNode);
    Q_ASSERT(node != NULL);

    if (node == m_firstNode)
        setLine(QLineF(node->getPos(), line().p2()));

    if (node == m_lastNode)
        setLine(QLineF(line().p1(), node->getPos()));
}

void CConnection::onNodeDetach(CNode *node)
{
	if (node == m_firstNode)
	{
		m_firstNode = NULL;
		return;
	}

	if (node == m_lastNode)
	{
		m_lastNode = NULL;
		return;
	}
}



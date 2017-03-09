#include "CConnection.h"

#include <QPen>


CConnection::CConnection(QGraphicsItem *parent): Super(parent)
{
    m_firstNode = m_lastNode = NULL;

    setPen(QPen(Qt::red));

    setZValue(-1);

    setFlags(QGraphicsItem::ItemIsSelectable);
}

CConnection::~CConnection()
{
	setFirstNode(NULL);
	setLastNode(NULL);
}


bool CConnection::storeTo(QDataStream & out) const
{
	Super::storeTo(out);

	out << quintptr(m_firstNode) << quintptr(m_lastNode);

	return true;
}


bool CConnection::restoreFrom(QDataStream & out)
{
	if (Super::restoreFrom(out))
	{
		// these are TEMP ids
		quintptr t1, t2; out >> t1 >> t2;
		m_firstNode = (CNode*)t1;
		m_lastNode = (CNode*)t2;

		return true;
	}

	return false;
}


bool CConnection::linkAfterRestore(const CItemLinkMap & idToItem)
{
	CNode *node1 = dynamic_cast<CNode*>(idToItem.value((quintptr)m_firstNode));
	CNode *node2 = dynamic_cast<CNode*>(idToItem.value((quintptr)m_lastNode));

	m_firstNode = m_lastNode = NULL;

	setFirstNode(node1);
	setLastNode(node2);

	return true;
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
}

void CConnection::reattach(CNode *oldNode, CNode *newNode)
{
	if (oldNode == newNode)
		return;

	if (m_firstNode == oldNode)
		setFirstNode(newNode);

	if (m_lastNode == oldNode)
		setLastNode(newNode);
}

CConnection* CConnection::clone()
{
	CConnection* c = new CConnection();
	c->setFirstNode(m_firstNode);
	c->setLastNode(m_lastNode);
	return c;
}

void CConnection::onNodeMoved(CNode *node)
{
    Q_ASSERT(node == m_firstNode || node == m_lastNode);
    Q_ASSERT(node != NULL);

    setLine(QLineF(m_firstNode->getPos(), m_lastNode->getPos()));
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



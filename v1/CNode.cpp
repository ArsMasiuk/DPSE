#include "CNode.h"
#include "CConnection.h"

#include <QDebug>

////////////////////////////////////////////////////////////////////
/// \brief CRectNode::CRectNode
/// \param parent

CRectNode::CRectNode(QGraphicsItem *parent): TNode<QGraphicsRectItem>(parent)
{
    setRect(-5,-5,11,11);
}

////////////////////////////////////////////////////////////////////
/// \brief CRoundNode::CRoundNode
/// \param parent

CRoundNode::CRoundNode(QGraphicsItem *parent): TNode<QGraphicsEllipseItem>(parent)
{
    setRect(-5,-5,11,11);
}


////////////////////////////////////////////////////////////////////
/// \brief CNode::CNode

CNode::~CNode()
{
	foreach (CConnection *conn, m_connections)
	{
		conn->onNodeDetach(this);
	}
}

///
/// \brief CNode::onConnectionAttach
/// \param conn
///
void CNode::onConnectionAttach(CConnection *conn)
{
    Q_ASSERT(conn != NULL);
    Q_ASSERT(!m_connections.contains(conn));

    m_connections.insert(conn);
}

///
/// \brief CNode::onConnectionDetach
/// \param conn
///
void CNode::onConnectionDetach(CConnection *conn)
{
    Q_ASSERT(conn != NULL);
    Q_ASSERT(m_connections.contains(conn));

    m_connections.remove(conn);
}

bool CNode::onMousePressed()
{
	return false;
}

void CNode::onPositionChanged()
{
    foreach (CConnection* conn, m_connections)
    {
        conn->onNodeMoved(this);
    }
}

void CNode::onMouseOver(QGraphicsSceneHoverEvent * event, QGraphicsItem * sourceItem)
{
	qDebug() << "hovered: " << sourceItem;
}

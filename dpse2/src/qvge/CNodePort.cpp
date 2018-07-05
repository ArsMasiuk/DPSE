/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CNodePort.h"
#include "CNode.h"


CNodePort::CNodePort(CNode *node, const QByteArray& portId, CNodePort::Anchor portAnchor, int portOrder) :
	Shape(dynamic_cast<QGraphicsItem*>(node)),
	m_node(node),
	m_id(portId), m_anchor(portAnchor), m_order(portOrder)
{
 	Q_ASSERT(m_node != NULL);

	setRect(-3, -3, 7, 7);
	setBrush(Qt::gray);
	setPen(QPen(Qt::black, 1));

	setToolTip(portId);

	setFlags(QGraphicsItem::ItemClipsToShape | QGraphicsItem::ItemIgnoresParentOpacity);
}


void CNodePort::onParentGeometryChanged()
{
	Q_ASSERT(m_node != NULL);

	QRectF nodeBox = m_node->Shape::boundingRect();

	switch (m_anchor)
	{
	case W: case NW: case SW:
		setX(nodeBox.left());
		break;
	case E: case NE: case SE:
		setX(nodeBox.right());
		break;
	default:
		setX(nodeBox.center().x());
		break;
	}

	switch (m_anchor)
	{
	case N: case NW: case NE:
		setY(nodeBox.top());
		break;
	case S: case SE: case SW:
		setY(nodeBox.bottom());
		break;
	default:
		setY(nodeBox.center().y());
		break;
	}
}


ItemDragTestResult CNodePort::acceptDragFromItem(QGraphicsItem* draggedItem)
{
	if (dynamic_cast<CNode*>(draggedItem))
		return Accepted;
	else
		return Ignored;
}


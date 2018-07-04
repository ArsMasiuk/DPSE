/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QGraphicsRectItem>

class CNode;


class CNodePort : public QGraphicsRectItem
{
public:
	typedef QGraphicsRectItem Shape;

	enum Anchor
	{
		N, S, E, W, NE, NW, SE, SW
	};

	explicit CNodePort(CNode *node, const QByteArray& portId, CNodePort::Anchor portAnchor = CNodePort::E, int portOrder = 0);
	virtual ~CNodePort() {}

	CNode* getNode() const				{ return m_node;	}
	const QByteArray& getId() const		{ return m_id;		}

	// callbacks
	virtual void onParentGeometryChanged();

protected:
	CNode *m_node;
	
	Anchor m_anchor;
	QByteArray m_id;
	int m_order;
};


/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QGraphicsRectItem>

#include "IInteractive.h"


class CNode;


class CNodePort : public QGraphicsRectItem, public IInteractive
{
public:
	typedef QGraphicsRectItem Shape;

	explicit CNodePort(CNode *node, const QByteArray& portId, int align, int xoff = 0, int yoff = 0);
	virtual ~CNodePort() {}

	CNode* getNode() const				{ return m_node;	}
	const QByteArray& getId() const		{ return m_id;		}

	// callbacks
	virtual void onParentGeometryChanged();

	virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem* draggedItem);
	virtual void leaveDragFromItem(QGraphicsItem* draggedItem);

protected:
	CNode *m_node;

	QByteArray m_id;
	int m_align;
	double m_xoff, m_yoff;
};


/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CControlPoint.h"
#include "CItem.h"


CControlPoint::CControlPoint(CItem *parent) : 
	Shape(dynamic_cast<QGraphicsItem*>(parent)),
	m_parentItem(parent)
{
 	Q_ASSERT(parent != NULL);

	setRect(-3, -3, 7, 7);
}


// reimp 

QVariant CControlPoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		m_parentItem->onControlPointMoved(this, value.toPointF());

		return value;
	}

	return Shape::itemChange(change, value);
}


/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QGraphicsRectItem>


class CItem;


class CControlPoint : public QGraphicsRectItem
{
public:
	typedef QGraphicsRectItem Shape;

	explicit CControlPoint(CItem *parent);
	virtual ~CControlPoint() {}

protected:
	// reimp 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

	CItem *m_parentItem;
};


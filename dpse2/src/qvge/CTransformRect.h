/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QGraphicsRectItem> 

class CItem;


class CTransformRect: public QObject
{
	Q_OBJECT

public:
	CTransformRect();
	~CTransformRect();

	virtual void paintTo(class CEditorScene *scene, QPainter *painter, const QRectF &r);

protected:
	struct ControlPoint
	{
		QPointF pos;
	};

	ControlPoint m_points[8];
};


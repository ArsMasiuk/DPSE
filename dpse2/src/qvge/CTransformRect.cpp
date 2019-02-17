/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include "CTransformRect.h"
#include "CItem.h"
#include "CEditorScene.h"


CTransformRect::CTransformRect()
{
}

CTransformRect::~CTransformRect()
{
}


void CTransformRect::paintTo(class CEditorScene *scene, QPainter *painter, const QRectF &)
{
	auto selItems = scene->transformableItems();
	if (selItems.size())
	{
		QRectF r;
		for (const auto& item : selItems)
		{
			r |= item->sceneBoundingRect();
		}

		scene->invalidate(r.adjusted(-5,-5,5,5));

		// update points
		m_points[0].pos = r.topLeft();
		m_points[1].pos = QPointF(r.center().x(), r.top());
		m_points[2].pos = r.topRight();
		m_points[3].pos = QPointF(r.left(), r.center().y());
		m_points[4].pos = QPointF(r.right(), r.center().y());
		m_points[5].pos = r.bottomLeft();
		m_points[6].pos = QPointF(r.center().x(), r.bottom());
		m_points[7].pos = r.bottomRight();

		painter->setPen(QPen(Qt::black, 0, Qt::SolidLine));
		painter->drawRect(r);

		for (int i = 0; i < 8; ++i)
			painter->fillRect(m_points[i].pos.x() - 4, m_points[i].pos.y() - 4, 9, 9, Qt::SolidPattern);
	}
}



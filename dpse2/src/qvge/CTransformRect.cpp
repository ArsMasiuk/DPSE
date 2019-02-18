/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include <QGraphicsView>

#include "CTransformRect.h"
#include "CItem.h"
#include "CEditorScene.h"


CTransformRect::CTransformRect()
{
	m_points[0].cursor = Qt::SizeFDiagCursor;
	m_points[1].cursor = Qt::SizeVerCursor;
	m_points[2].cursor = Qt::SizeBDiagCursor;
	m_points[3].cursor = Qt::SizeHorCursor;
	m_points[4].cursor = Qt::SizeHorCursor;
	m_points[5].cursor = Qt::SizeFDiagCursor;
	m_points[6].cursor = Qt::SizeVerCursor;
	m_points[7].cursor = Qt::SizeBDiagCursor;
}

CTransformRect::~CTransformRect()
{
}

void CTransformRect::onActivated(CEditorScene& scene)
{
	scene.update();
}

void CTransformRect::draw(class CEditorScene &scene, QPainter *painter, const QRectF &)
{
	auto selItems = scene.transformableItems();
	if (selItems.size())
	{
		QRectF r;
		for (auto item : selItems)
		{
			r |= item->sceneBoundingRect();
		}

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

		auto view = scene.getCurrentView();
		if (view)
		{
			for (int i = 0; i < 8; ++i)
			{
				// zoom-independend control points
				QPoint viewPos = view->mapFromScene(m_points[i].pos);
				QPointF topLeft = view->mapToScene(QPoint(viewPos.x() - 4, viewPos.y() - 4));
				QPointF bottomRight = view->mapToScene(QPoint(viewPos.x() + 4, viewPos.y() + 4));

				painter->fillRect(QRectF(topLeft, bottomRight), Qt::SolidPattern);
			}

			scene.invalidate(r.adjusted(-5, -5, 5, 5));
		}

	}
}



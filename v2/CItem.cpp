#include "CItem.h"

#include <QGraphicsSceneMouseEvent>


bool CItem::acceptMousePressed(QGraphicsSceneMouseEvent * mouseEvent)
{
	return true;
}

void CItem::mouseReleased(QGraphicsSceneMouseEvent * mouseEvent)
{
}

bool CItem::acceptDragStarted(QGraphicsSceneMouseEvent * mouseEvent, CItem * itemUnderMouse)
{
	if (itemFlags() & QGraphicsItem::ItemIsMovable)
		return true;

	return false;
}

void CItem::mouseDragged(QGraphicsSceneMouseEvent * mouseEvent, CItem * itemUnderMouse)
{
	moveTo(mouseEvent->scenePos());
}

bool CItem::acceptMousePressedRight(QGraphicsSceneMouseEvent * mouseEvent)
{
	return true;
}

bool CItem::storeTo(QDataStream & out) const
{
	out << getPos() << itemFlags();

	return true;
}

bool CItem::restoreFrom(QDataStream & out)
{
	if (!out.atEnd())
	{
		QPointF p; out >> p; moveTo(p);
		int f; out >> f; setItemFlags(f);

		return true;
	}

	return false;
}

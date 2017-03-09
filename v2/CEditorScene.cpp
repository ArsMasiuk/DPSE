#include "CEditorScene.h"
#include "CItem.h"
#include "IDragController.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>


CEditorScene::CEditorScene(QObject *parent): QGraphicsScene(parent), 
	m_pressedItem(NULL),
	m_pressedItemRight(NULL),
	m_releasedItem(NULL),
	m_sceneController(NULL)
{
    m_gridSize = 25;
    m_gridEnabled = true;
    m_gridSnap = true;
    m_gridPen = QPen(Qt::gray, 0, Qt::DotLine);

    setSceneRect(0,0, 500,500);
}

void CEditorScene::setGridSize(int newSize)
{
    if (newSize <= 0)
        return;

    m_gridSize = newSize;

    update();
}

void CEditorScene::enableGrid(bool on)
{
    m_gridEnabled = on;

    update();
}

void CEditorScene::enableGridSnap(bool on)
{
    m_gridSnap = on;
}

void CEditorScene::setGridPen(const QPen &gridPen)
{
    m_gridPen = gridPen;

    update();
}

void CEditorScene::setSceneController(IDragController * c)
{
	if (m_sceneController)
		m_sceneController->onSceneDetached(this);

	m_sceneController = c;

	if (m_sceneController)
		m_sceneController->onSceneAttached(this);
}

// protected

void CEditorScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(Qt::darkGray, 2, Qt::SolidLine));
    QColor fillColor("lightsteelblue");
    fillColor.setAlpha(50);
    painter->setBrush(fillColor);
    painter->drawRect(sceneRect());

    if (m_gridSize < 0 || !m_gridEnabled)
        return;

    painter->setPen(m_gridPen);

    qreal left = int(rect.left()) - (int(rect.left()) % m_gridSize);
    qreal top = int(rect.top()) - (int(rect.top()) % m_gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += m_gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += m_gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    //qDebug() << lines.size();

    painter->drawLines(lines.data(), lines.size());
}

void CEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	// if nothing dragged & pressed: run super to accept hovers
	//if (!m_draggedItem && !m_pressedItem)
	//{
	//	Super::mouseMoveEvent(mouseEvent);
	//	return;
	//}

	QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
	CItem *citem = dynamic_cast<CItem*>(item);

	if (m_sceneController)
	{
		m_sceneController->acceptMouseMoved(this, citem, mouseEvent);
	}
}

void CEditorScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	// right button check
	if (mouseEvent->button() == Qt::RightButton)
	{
		scenePressedRight(mouseEvent);
		return;
	}

	m_pressedItem = NULL;

	QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
	CItem *citem = dynamic_cast<CItem*>(item);

	if (m_sceneController)
	{
		if (m_sceneController->acceptMousePressed(this, citem, mouseEvent))
		{
			m_pressedItem = citem;
		}
	}
	else
	{
		m_pressedItem = citem;
	}
}

void CEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	// right button check
	if (mouseEvent->button() == Qt::RightButton)
	{
		sceneReleasedRight(mouseEvent);
		return;
	}

	QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
	m_releasedItem = dynamic_cast<CItem*>(item);

	if (m_sceneController)
	{
		m_sceneController->acceptMouseReleased(this, m_releasedItem, mouseEvent);
	}

	if (!m_pressedItem)
	{
		sceneReleased(mouseEvent);
	}
	
	m_pressedItem = NULL;
}

void CEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	// default call
	mousePressEvent(mouseEvent);
}

bool CEditorScene::acceptMousePressed(CItem * citem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (m_sceneController)
		return m_sceneController->acceptMousePressed(this, citem, mouseEvent);

	return true;
}

bool CEditorScene::acceptMousePressedRight(CItem * citem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (m_sceneController)
		return m_sceneController->acceptMousePressedRight(this, citem, mouseEvent);

	return true;
}

void CEditorScene::scenePressedRight(QGraphicsSceneMouseEvent * mouseEvent)
{
	m_pressedItemRight = NULL;

	QGraphicsItem *item = itemAt(mouseEvent->scenePos(), QTransform());
	CItem *citem = dynamic_cast<CItem*>(item);

	if (acceptMousePressedRight(citem, mouseEvent))
	{
		if (citem)
		{
			if (citem->acceptMousePressedRight(mouseEvent))
			{
				m_pressedItemRight = citem;

				onItemPressedRight(m_pressedItemRight, mouseEvent);
			}
		}
		else
			onScenePressedRight(mouseEvent);
	}
}

bool CEditorScene::storeTo(QDataStream& out) const
{
	QList<QGraphicsItem*> allItems = items();
	foreach(QGraphicsItem* item, allItems)
	{
		CItem* citem = dynamic_cast<CItem*>(item);
		if (citem)
		{
			out << citem->typeId() << quintptr(citem);

			citem->storeTo(out);
		}
	}

	return true;
}


bool CEditorScene::restoreFrom(QDataStream& out)
{
	if (m_sceneController)
		m_sceneController->reset();

	clear();

	CItem::CItemLinkMap idToItem;

	// read
	while (!out.atEnd())
	{
		QByteArray id; out >> id;
		quintptr ptrId; out >> ptrId;

		CItem* item = createItemOfType(id);

		if (item)
		{
			if (item->restoreFrom(out))
			{
				idToItem[ptrId] = item;
				continue;
			}
		}

		// failed: cleanup
		qDeleteAll(idToItem.values());

		return false;
	}

	// link items
	foreach(CItem* item, idToItem.values())
	{
		if (item->linkAfterRestore(idToItem))
		{
			addItem(dynamic_cast<QGraphicsItem*>(item));
		}
		else
		{
			// failed: cleanup
			qDeleteAll(idToItem.values());

			clear();

			return false;
		}

	}

	return true;
}

void CEditorScene::onItemDestroyed(CItem * citem)
{
	Q_ASSERT(citem);

	if (m_pressedItem == citem)
		m_pressedItem = NULL;

	if (m_pressedItemRight == citem)
		m_pressedItemRight = NULL;

	if (m_releasedItem == citem)
		m_releasedItem = NULL;
}


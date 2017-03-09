#ifndef CITEM_H
#define CITEM_H

#include <QGraphicsItem>

#include "CEditorScene.h"


class CItem
{
public:
	// to be reimplemented
	virtual QByteArray typeId() const { return "CItem"; }
	virtual QPointF getPos() const = 0;
	virtual void moveTo(const QPointF &pos) = 0;
	virtual int itemFlags() const = 0;
	virtual void setItemFlags(int f) = 0;

	// serialization 
	virtual bool storeTo(QDataStream& out) const;
	virtual bool restoreFrom(QDataStream& out);

	typedef QMap<quintptr, CItem*> CItemLinkMap;
	virtual bool linkAfterRestore(const CItemLinkMap& idToItem) { return true; }

	// returns new item of this class
	virtual CItem* clone() = 0;
	virtual CItem* create() const = 0;

	virtual bool acceptMousePressed(QGraphicsSceneMouseEvent * mouseEvent);
	virtual void mouseReleased(QGraphicsSceneMouseEvent * mouseEvent);

	virtual bool acceptDragStarted(QGraphicsSceneMouseEvent * mouseEvent, CItem * itemUnderMouse);
	virtual void mouseDragged(QGraphicsSceneMouseEvent * mouseEvent, CItem * itemUnderMouse);

	virtual bool acceptMousePressedRight(QGraphicsSceneMouseEvent * mouseEvent);

	// callbacks
	virtual void onItemMoved() {}

protected:
	virtual CEditorScene* getScene() const = 0;
};


template<class ShapeItem, class Item>
class TItem : public ShapeItem, virtual public Item
{
public:
	TItem(QGraphicsItem *parent = Q_NULLPTR) : ShapeItem(parent)
	{
	}

	virtual TItem::~TItem()
	{
		CEditorScene *scene = this->getScene();
		if (scene)
			scene->onItemDestroyed(this);
	}

	virtual QPointF getPos() const
	{
		return ShapeItem::pos();
	}

	virtual void moveTo(const QPointF &pos)
	{
		QPointF newPos = pos;

		CEditorScene *editorScene = getScene();
		if (editorScene)
		{
			if (editorScene->gridSnapEnabled())
			{
				int snapStep = editorScene->getGridSize();

				if (newPos.x() < 0)
					newPos.setX(newPos.x() - snapStep / 2);
				else
					newPos.setX(newPos.x() + snapStep / 2);

				if (newPos.y() < 0)
					newPos.setY(newPos.y() - snapStep / 2);
				else
					newPos.setY(newPos.y() + snapStep / 2);

				newPos.setX((int)newPos.x() - (int)newPos.x() % snapStep);
				newPos.setY((int)newPos.y() - (int)newPos.y() % snapStep);
			}
		}

		ShapeItem::setPos(newPos);

		onItemMoved();
	}

	virtual int itemFlags() const
	{
		return flags();
	}

	virtual void setItemFlags(int f)
	{
		setFlags((QGraphicsItem::GraphicsItemFlags)f);
	}

	virtual CItem* clone()
	{
		CItem *item = create();

		*item = *this;	// copy constructor

		TItem *titem = dynamic_cast<TItem*>(item);
		
		if (ShapeItem::scene())
			ShapeItem::scene()->addItem(titem);

		return item;
	}

protected:
	virtual CEditorScene* getScene() const
	{
		return dynamic_cast<CEditorScene*>(ShapeItem::scene());
	}
};


#endif // CITEM_H

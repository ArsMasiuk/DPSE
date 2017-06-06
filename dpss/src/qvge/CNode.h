/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CNODE_H
#define CNODE_H

#include "CItem.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QSet>

 
class CConnection;


enum NodeFlags
{
	NF_OrphanAllowed = 1	// allow node to have no connections
};


class CNode : public CItem, public QGraphicsRectItem
{
public:
	typedef CItem Super;
	typedef QGraphicsRectItem Shape;

	CNode(QGraphicsItem* parent = NULL);
	virtual ~CNode();

	static QByteArray factoryId()		{ return "CNode"; }
	virtual QByteArray typeId() const	{ return this->factoryId(); }
	virtual QString createNewId() const;

	int nodeFlags() const				{ return m_nodeFlags; }
	void setNodeFlags(int f)			{ m_nodeFlags = f; }
	void setNodeFlag(int f)				{ m_nodeFlags |= f; }
	void resetNodeFlag(int f)			{ m_nodeFlags &= ~f; }

	// reimp
	virtual CItem* create() const		{ return new CNode(parentItem()); }
	virtual CItem* clone();

	virtual float getSize() const		{ return rect().width(); }
	virtual void resize(float size)		{ setRect(-size / 2, -size / 2, size, size); }

	virtual bool setAttribute(const QByteArray& attrId, const QVariant& v);
	virtual QVariant getAttribute(const QByteArray& attrId) const;
    virtual QByteArray classId() const { return "node"; }
    virtual QByteArray superClassId() const { return Super::classId(); }

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);

	// merges node with the current one.
	// node will be deleted afterwards.
	// if allowCircled = false then all the connections from node to this will be removed as well.
	virtual void merge(CNode *node, bool allowCircled = false);

	// splits all the connections from this node.
	// result is the list of the newly created nodes (or empty list if connections < 2).
	virtual QList<CNode*> unlink();

	// returns all nodes colliding with this one.
	QList<CNode*> getCollidingNodes() const;
	
	// returns all connections of the node.
	QSet<CConnection*> getConnections() const { return m_connections; }

	// returns true if new connection from this node is allowed.
	virtual bool allowStartConnection() const;

	// calculates distance to the line's end point (used to draw connections to this item).
	virtual double getDistanceToLineEnd(const QLineF& line) const;

	// callbacks
	virtual void onConnectionAttach(CConnection *conn);
	virtual void onConnectionDetach(CConnection *conn);
	virtual void onConnectionDeleted(CConnection *conn);

	virtual void onItemMoved();
	virtual void onDroppedOn(const QSet<CItem*>& acceptedItems, const QSet<CItem*>& rejectedItems);

protected:
	virtual void copyDataFrom(CItem* from);

	// reimp 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
	virtual QRectF boundingRect() const;
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event); 

	virtual void updateLabelPosition();

private:
	void updateConnections();

protected:
	QSet<CConnection*> m_connections;
	int m_nodeFlags;

	QPolygonF m_shapeCache;
	QRectF m_sizeCache;
};



#endif // CNODE_H

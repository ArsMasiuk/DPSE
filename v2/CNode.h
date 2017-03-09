#ifndef CNODE_H
#define CNODE_H

#include <CItem.h>

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QSet>

 
class CConnection;


class CNode : public CItem
{
public:
	typedef CItem Super;

	virtual ~CNode();
	virtual QByteArray typeId() const { return "CNode"; }

	// merges node with the current one.
	// node will be deleted afterwards.
	// if allowCircled = false then all the connections from node to this will be removed as well.
	void merge(CNode *node, bool allowCircled = false);

	// splits all the connections from this node.
	// result is the list of the newly created nodes (or empty list if connections < 2).
	QList<CNode*> unlink();

	// returns all nodes colliding with this one.
	QList<CNode*> getCollidingNodes() const;
	
	// returns all connections of the node.
	QSet<CConnection*> getConnections() const { return m_connections; }

	// callbacks
	virtual void onConnectionAttach(CConnection *conn);
	virtual void onConnectionDetach(CConnection *conn);
	virtual void onItemMoved();

protected:
	QSet<CConnection*> m_connections;
};


class CRectNode : public TItem<QGraphicsRectItem, CNode>
{
public:
	typedef TItem<QGraphicsRectItem, CNode> Super;

	CRectNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual QByteArray typeId() const { return "CRectNode"; }

	virtual CItem* create() const
	{
		return new CRectNode(parentItem());
	}
};


class CRoundNode : public TItem<QGraphicsEllipseItem, CNode>
{
public:
	typedef TItem<QGraphicsEllipseItem, CNode> Super;

	CRoundNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual QByteArray typeId() const { return "CRoundNode"; }

	virtual CItem* create() const
	{
		return new CRoundNode(parentItem());
	}

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};



#endif // CNODE_H

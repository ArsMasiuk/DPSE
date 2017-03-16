#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QGraphicsLineItem>
#include <QByteArray>

#include "CItem.h"

class CNode;


enum ConnectionFlags	// extends ItemFlags
{
	CF_Start_Arrow		= IF_LastFlag,
	CF_End_Arrow		= IF_LastFlag << 2,
	CF_Mutual_Arrows	= CF_Start_Arrow | CF_End_Arrow		// start | end
};


class CConnection : public CItem, public QGraphicsLineItem
{
public:
	typedef CItem Super;
	typedef QGraphicsLineItem Shape;

    CConnection(QGraphicsItem *parent = Q_NULLPTR);
	virtual ~CConnection();

	// public
    void setFirstNode(CNode *node);
    void setLastNode(CNode *node);
	void setBendFactor(int bf);

	void reattach(CNode *oldNode, CNode *newNode);
	void reverse();

	CNode* firstNode() const { return m_firstNode; }
	CNode* lastNode() const { return m_lastNode; }

	bool isValid() const	{ return m_firstNode != NULL && m_lastNode != NULL; }
	bool isCircled() const	{ return isValid() && m_firstNode == m_lastNode; }

	// reimp
	static QByteArray factoryId()			{ return "CConnection"; }
	virtual QByteArray typeId() const		{ return this->factoryId(); }
    virtual QByteArray classId() const		{ return "edge"; }
    virtual QByteArray superClassId() const { return Super::classId(); }

	virtual QString createNewId() const;
	virtual CItem* create() const			{ return new CConnection(parentItem());	}
	CConnection* clone();

	// reimp
	virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem* /*draggedItem*/) { return Ignored; }

	// reimp
	virtual QPainterPath shape() const;

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);
	virtual bool linkAfterRestore(const CItemLinkMap& idToItem);

    // external callbacks 
    void onNodeMoved(CNode *node);
	void onNodeDetached(CNode *node);
	void onNodeDeleted(CNode *node);

protected:
	virtual void drawArrow(QPainter *painter, const QStyleOptionGraphicsItem *option, bool first, const QLineF& direction);

	// internal callbacks
	virtual void onPositionUpdated();

	// reimp
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
	virtual QPointF labelOffset(const QRectF& itemRect, const QSizeF& labelSize) const;

protected:
    CNode *m_firstNode, *m_lastNode;
	
	QPainterPath m_shapePath;
	int m_bendFactor;
	QPointF m_controlPoint, m_controlPos;
};



/// CLineItems evaluator

class CLineItems : public CItemsEvaluator
{
public:
	virtual bool evaluate(const QGraphicsItem& item) const
	{
		return dynamic_cast<const CConnection*>(&item) != NULL;
	}
};


#endif // CCONNECTION_H

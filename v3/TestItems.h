#pragma once

#include "qvge/CNode.h"


/* CTestRectNode 
*/
class CTestRectNode : public CNode
{
public:
	typedef CNode Super;

	CTestRectNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual QByteArray typeId() const { return "CTestRectNode"; }

	virtual CItem* create() const
	{
		return new CTestRectNode(parentItem());
	}

	// call from drag event
	virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem* draggedItem);
};


/* CTestRoundNode
*/
class CTestRoundNode : public CNode
{
public:
	typedef CNode Super;

	CTestRoundNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual QByteArray typeId() const { return "CTestRoundNode"; }

	virtual CItem* create() const
	{
		return new CTestRoundNode(parentItem());
	}

	// call from drag event
	virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem* draggedItem);
	virtual void leaveDragFromItem(QGraphicsItem* draggedItem);
};


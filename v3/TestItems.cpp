#include "TestItems.h"



CTestRectNode::CTestRectNode(QGraphicsItem * parent) : Super(parent)
{
	setPen(QPen(Qt::blue));
	setBrush(Qt::green);

	setFlag(ItemIsMovable, false);
}


ItemDragTestResult CTestRectNode::acceptDragFromItem(QGraphicsItem * draggedItem)
{
	if (!dynamic_cast<CNode*>(draggedItem))
	{
		return Rejected;
	}

	return Accepted;
}


/// CTestRoundNode

CTestRoundNode::CTestRoundNode(QGraphicsItem *parent) : Super(parent)
{
	setPen(QPen(Qt::blue));
	setBrush(Qt::yellow);

	QGraphicsTextItem *labelPtr = new QGraphicsTextItem(this);
	labelPtr->setFlags(QGraphicsItem::ItemIgnoresTransformations | QGraphicsItem::ItemStacksBehindParent);
	labelPtr->setHtml("Label :) <hr><b>html</b>");
	labelPtr->setTextInteractionFlags(Qt::TextEditorInteraction);
}



// call from drag event

ItemDragTestResult CTestRoundNode::acceptDragFromItem(QGraphicsItem* draggedItem)
{
	if (!dynamic_cast<CNode*>(draggedItem))
	{
		return Rejected;
	}

	setRect(-7, -7, 15, 15);

	return Accepted;
}


void CTestRoundNode::leaveDragFromItem(QGraphicsItem* draggedItem)
{
	setRect(-5, -5, 11, 11);
}



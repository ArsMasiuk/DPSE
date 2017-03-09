#pragma once
#ifndef CDRAGCONTROLLER_H
#define CDRAGCONTROLLER_H

#include <IDragController.h>


class CDragController : public IDragController 
{
public:
	CDragController();
	virtual ~CDragController();

	// drag control
	void setDragItem(CItem *citem);
	CItem* getDraggedItem() const		{ return m_draggedItem; }

	// reset controller state (cancel current operation)
	virtual void reset();

	// reimp (IDragController)
	virtual void onSceneAttached(CEditorScene * scene);
	virtual void onSceneDetached(CEditorScene * scene);
	virtual void onItemDropped(CEditorScene * scene, CItem * droppedItem, CItem * targetItem) {}

	virtual bool acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool acceptMousePressedRight(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool acceptMouseMoved(CEditorScene * scene, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual CItem* acceptDragStarted(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool acceptMouseReleased(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual void dragFinished(CEditorScene * scene, CItem * draggedItem, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);

protected:
	// reimplement to extend action cancellation
	virtual bool isActionInProgress() const;

private:
	CItem *m_draggedItem;
};

#endif // CDRAGCONTROLLER_H
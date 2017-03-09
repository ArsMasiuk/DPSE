#pragma once

class QGraphicsSceneMouseEvent;

class CItem;
class CEditorScene;

class IDragController
{
public:
	virtual void reset() = 0;

	virtual void onSceneAttached(CEditorScene * scene) = 0;
	virtual void onSceneDetached(CEditorScene * scene) = 0;
	virtual void onItemDropped(CEditorScene * scene, CItem * droppedItem, CItem * targetItem) = 0;

	virtual bool acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
	virtual bool acceptMousePressedRight(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
	virtual bool acceptMouseMoved(CEditorScene * scene, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
	virtual CItem* acceptDragStarted(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
	virtual bool acceptMouseReleased(CEditorScene * scene, CItem * releasedItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
	virtual void dragFinished(CEditorScene * scene, CItem * draggedItem, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent) = 0;
};


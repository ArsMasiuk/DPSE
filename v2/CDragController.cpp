#include "CDragController.h"
#include "CItem.h"


CDragController::CDragController():
	m_draggedItem(NULL)
{
	
}

CDragController::~CDragController() 
{
	
}

void CDragController::setDragItem(CItem * citem)
{
	m_draggedItem = citem;
}


// reimp

void CDragController::reset()
{
	m_draggedItem = NULL;
}

void CDragController::onSceneAttached(CEditorScene * scene)
{
	reset();
}

void CDragController::onSceneDetached(CEditorScene * scene)
{
	reset();
}

bool CDragController::acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (pressedItem)
		return pressedItem->acceptMousePressed(mouseEvent);

	return false;
}

bool CDragController::acceptMousePressedRight(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (isActionInProgress())
	{
		reset();
		return false;
	}

	return true;
}

bool CDragController::acceptMouseMoved(CEditorScene * scene, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	// if dragging:
	if (m_draggedItem)
	{
 		m_draggedItem->mouseDragged(mouseEvent, hoverItem);
		return true;
	}

	// check if it is needed to start a drag:
	CItem *pressedItem = scene->getPressedItem();

	m_draggedItem = acceptDragStarted(scene, pressedItem, hoverItem, mouseEvent);

	return (m_draggedItem != NULL);
}

CItem* CDragController::acceptDragStarted(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (pressedItem)
		return pressedItem->acceptDragStarted(mouseEvent, hoverItem) ? pressedItem : NULL;

	return NULL;
}

bool CDragController::acceptMouseReleased(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (m_draggedItem)
	{
		dragFinished(scene, m_draggedItem, pressedItem, mouseEvent);

		m_draggedItem = NULL;

		return true;
	}

	return false;
}

void CDragController::dragFinished(CEditorScene * scene, CItem * draggedItem, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
}

bool CDragController::isActionInProgress() const
{
	return (m_draggedItem != nullptr);
}

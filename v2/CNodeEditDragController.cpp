#include "CNodeEditDragController.h"
#include "CNode.h"

CNodeEditDragController::CNodeEditDragController()
	: Super()
{
	m_clicked = false;
	m_creating = false;
	m_lastClickedItem = NULL;
}

CNodeEditDragController::~CNodeEditDragController()
{

}

void CNodeEditDragController::onItemDropped(CEditorScene * scene, CItem * droppedItem, CItem * targetItem)
{
	// if dropped on another item: end with creation
	if (targetItem)
		m_creating = false;
}

bool CNodeEditDragController::acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	m_clicked = false;
	m_pressPos = mouseEvent->scenePos();
	m_releasePos = QPointF();

	// default behaviour
	return SuperBasic::acceptMousePressed(scene, pressedItem, mouseEvent);
}


bool CNodeEditDragController::acceptMouseReleased(CEditorScene * scene, CItem * releasedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	m_clicked = false;
	m_releasePos = mouseEvent->scenePos();

	if (m_releasePos == m_pressPos)
	{
		m_clicked = true;

		acceptMouseClicked(scene, releasedItem, mouseEvent);
	}

	return Super::acceptMouseReleased(scene, releasedItem, mouseEvent);
}


bool CNodeEditDragController::acceptMouseMoved(CEditorScene * scene, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	// drag operation done by scene
	if (Super::acceptMouseMoved(scene, hoverItem, mouseEvent))
	{
		return true;
	}

	// no item dragged
	if (m_creating)
	{
		CItem* lastReleasedItem = scene->getReleasedItem();

		Super::startNewDrag(scene, lastReleasedItem, mouseEvent);
	}

	return true;
}


bool CNodeEditDragController::acceptMouseClicked(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (m_lastClickedItem && m_lastClickedItem == pressedItem)
	{
		m_creating = false;
	}
	else
	{
		m_lastClickedItem = pressedItem;
		m_creating = true;
	}

	return true;
}


bool CNodeEditDragController::movedAfterClick(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent)
{
	if (!m_creating)
	{
		return false;
	}

	return Super::startNewDrag(scene, pressedItem, mouseEvent);
}

void CNodeEditDragController::doCleanup()
{
	Super::doCleanup();

	m_creating = false;
	m_clicked = false;

	m_lastClickedItem = NULL;
	m_pressPos = m_releasePos = QPointF();
}

void CNodeEditDragController::setCreating(bool on)
{
	m_creating = on;
}

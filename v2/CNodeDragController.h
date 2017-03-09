#pragma once
#include "CDragController.h"

class CNode;
class CConnection;


class CNodeDragController :	public CDragController
{
public:
	typedef CDragController Super;

	CNodeDragController();
	virtual ~CNodeDragController();

	void setCreateMode(bool on);

	// factory call to create new node from pressed one
	virtual CNode* createNode(CNode * pressedNode = nullptr);

	// reimp
	virtual void reset();

	virtual bool acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual CItem* acceptDragStarted(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual void dragFinished(CEditorScene * scene, CItem * draggedItem, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);

protected:
	virtual bool startNewDrag(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual void doCleanup();

private:
	CNode *m_tempStartNode, *m_tempEndNode;
	CConnection *m_tempConnection;
	bool m_tempStartReal;

	bool m_createMode;
};


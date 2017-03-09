#ifndef CNODEEDITDRAGCONTROLLER_H
#define CNODEEDITDRAGCONTROLLER_H

#include <QPointF>
#include <QGraphicsSceneMouseEvent>

#include <CNodeDragController.h>

class CNodeEditDragController : public CNodeDragController
{
public:
	typedef CNodeDragController Super;
	typedef CDragController SuperBasic;

	CNodeEditDragController();
	~CNodeEditDragController();

	// reimp
	virtual void onItemDropped(CEditorScene * scene, CItem * droppedItem, CItem * targetItem);

	virtual bool acceptMousePressed(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool acceptMouseReleased(CEditorScene * scene, CItem * releasedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool acceptMouseMoved(CEditorScene * scene, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent);

	virtual bool acceptMouseClicked(CEditorScene * scene, CItem * pressedItem, QGraphicsSceneMouseEvent * mouseEvent);
	virtual bool movedAfterClick(CEditorScene * scene, CItem * pressedItem, CItem * hoverItem, QGraphicsSceneMouseEvent * mouseEvent);

protected:
	// reimp
	virtual void doCleanup();

private:
	void setCreating(bool on);

	QPointF m_pressPos;
	QPointF m_releasePos;
	bool m_clicked;
	bool m_creating;
	CItem *m_lastClickedItem;
};

#endif // CNODEEDITDRAGCONTROLLER_H

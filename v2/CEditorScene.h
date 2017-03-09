#ifndef CEDITORSCENE_H
#define CEDITORSCENE_H

#include <QGraphicsScene>

class CItem;
class IDragController;

class CEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
	typedef QGraphicsScene Super;

    CEditorScene(QObject *parent);

    void setGridSize(int newSize);
    int getGridSize() const				{ return m_gridSize; }

    bool gridEnabled() const			{ return m_gridEnabled; }
    bool gridSnapEnabled() const        { return m_gridSnap; }

    void setGridPen(const QPen& gridPen);
    const QPen& getGridPen() const      { return m_gridPen; }

	void setSceneController(IDragController *c);

	// getters
	CItem* getPressedItem() const		{ return m_pressedItem; }
	CItem* getPressedItemRight() const	{ return m_pressedItemRight; }
	CItem* getReleasedItem() const		{ return m_releasedItem; }

	// serialization 
	virtual bool storeTo(QDataStream& out) const;
	virtual bool restoreFrom(QDataStream& out);

	// items factory
	virtual CItem* createItemOfType(const QByteArray& tid) const { return NULL;  }

	// callbacks
	virtual void onItemDestroyed(CItem *citem);

public Q_SLOTS:
    void enableGrid(bool on = true);
    void enableGridSnap(bool on = true);

protected:
	virtual void drawBackground(QPainter *painter, const QRectF &rect);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);

	virtual bool acceptMousePressed(CItem *citem, QGraphicsSceneMouseEvent *mouseEvent);
	//virtual bool acceptDragStarted(CItem *citem, QGraphicsSceneMouseEvent * mouseEvent, CItem * itemUnderMouse);
	//virtual void dragFinished(CItem *citem, QGraphicsSceneMouseEvent *mouseEvent) {}
	virtual bool acceptMousePressedRight(CItem *citem, QGraphicsSceneMouseEvent *mouseEvent);

	virtual void scenePressed(QGraphicsSceneMouseEvent *mouseEvent) {}
	virtual void sceneReleased(QGraphicsSceneMouseEvent *mouseEvent) {}
	virtual void scenePressedRight(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void sceneReleasedRight(QGraphicsSceneMouseEvent *mouseEvent) {}

	// callbacks
	virtual void onScenePressedRight(QGraphicsSceneMouseEvent *mouseEvent) {}
	virtual void onItemPressedRight(CItem *citem, QGraphicsSceneMouseEvent *mouseEvent) {}

protected:
    int m_gridSize;
    bool m_gridEnabled;
    bool m_gridSnap;
    QPen m_gridPen;

	CItem *m_pressedItem; 
	CItem *m_pressedItemRight;
	CItem *m_releasedItem;

	IDragController *m_sceneController;
};

#endif // CEDITORSCENE_H

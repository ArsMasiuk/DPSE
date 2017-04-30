#ifndef CEDITORSCENE_H
#define CEDITORSCENE_H

#include <QGraphicsScene>
#include <QSet>
#include <QMenu>
#include <QByteArrayList>

#include <CAttribute.h>


class CItem;
class IUndoManager;


class CItemsEvaluator
{
public:
	virtual bool evaluate(const QGraphicsItem& item) const = 0;
};


class CDeletableItems: public CItemsEvaluator
{
public:
	virtual bool evaluate(const QGraphicsItem& item) const;
};


class CEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
	typedef QGraphicsScene Super;

    CEditorScene(QObject *parent);
	virtual ~CEditorScene();

	virtual void reset();
	virtual void initialize();
	virtual void activate();

	// properties
    void setGridSize(int newSize);
    int getGridSize() const				{ return m_gridSize; }

    bool gridEnabled() const			{ return m_gridEnabled; }
    bool gridSnapEnabled() const        { return m_gridSnap; }

    void setGridPen(const QPen& gridPen);
    const QPen& getGridPen() const      { return m_gridPen; }

	void setSceneCursor(const QCursor& c) { m_sceneCursor = c; }

	bool itemLabelsEnabled() const		{ return m_labelsEnabled; }
	bool itemLabelsNeedUpdate() const	{ return m_labelsUpdate; }

	// undo-redo
	int availableUndoCount() const;
	int availableRedoCount() const;
	// must be called after scene state changed
	void addUndoState();

	// serialization 
	virtual bool storeTo(QDataStream& out) const;
	virtual bool restoreFrom(QDataStream& out);

	// items factory
	template<class T>
	bool registerItemFactory()
	{
		static T f;
		return addItemFactory(&f);
	}

	bool addItemFactory(CItem *factoryItem);

	CItem* activateItemFactory(const QByteArray& factoryId);

	virtual CItem* createItemOfType(const QByteArray& typeId) const;

	template<class T>
	T* createItemOfType(QPointF* at = NULL) const;

	CItem* getActiveItemFactory() const {
		return m_activeItemFactory;
	}

	// attributes
    QVariant getClassAttribute(const QByteArray& classId, const QByteArray& attrId) const {
        return m_classAttributes[classId][attrId].defaultValue;
    }

	void setClassAttribute(const CAttribute& attr, bool vis = false);

	AttributesMap getClassAttributes(const QByteArray& classId, bool inherited) const;
	
	QByteArrayList getClasses() const {
		return m_classAttributes.keys();
	}

	QSet<QByteArray> getVisibleClassAttributes(const QByteArray& classId, bool inherited) const;

	void setClassAttributeVisible(const QByteArray& classId, const QByteArray& attrId, bool vis = true);

	QByteArray getSuperClassId(const QByteArray& classId) const {
		if (m_classToSuperIds.contains(classId))
			return m_classToSuperIds[classId];

		return QByteArray();
	}

	// selections
	QList<QGraphicsItem*> createSelectedList(const CItemsEvaluator&) const;

	template<class T, class L = T>
	QList<L*> getSelectedItems(bool triggeredIfEmpty = false) const;

	template<class T, class L = T>
	QList<L*> getItems() const;

	// callbacks
	virtual void onItemDestroyed(CItem *citem);

	// other
	bool checkLabelRegion(const QRectF& r);

public Q_SLOTS:
    void enableGrid(bool on = true);
    void enableGridSnap(bool on = true);
	void enableItemLabels(bool on = true);

	void undo();
	void redo();

	virtual void onActionDelete();
	virtual void onActionSelectAll();

	void selectAll();
	void deselectAll();

Q_SIGNALS:
	void undoAvailable(bool);
	void redoAvailable(bool);

	void sceneChanged();

protected:
	// reimp
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
	virtual void drawForeground(QPainter *painter, const QRectF &rect);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);
	virtual void keyPressEvent(QKeyEvent *keyEvent);

	// to reimplement
	virtual bool populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems);

	// call from reimp
	void moveDrag(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem* dragItem);
	void finishDrag(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem* dragItem, bool dragCancelled);

	virtual void onDragging(QGraphicsItem* dragItem, const QSet<CItem*>& acceptedItems, const QSet<CItem*>& rejectedItems);
	virtual void onMoving(QGraphicsSceneMouseEvent* mouseEvent, QGraphicsItem* hoverItem);
	virtual void onLeftClick(QGraphicsSceneMouseEvent* /*mouseEvent*/, QGraphicsItem* /*clickedItem*/) {}
	virtual void onLeftDoubleClick(QGraphicsSceneMouseEvent* mouseEvent, QGraphicsItem* clickedItem);

private:
	void updateSceneCursor();
	void removeItems();
	void checkUndoState();

protected:
	QPointF m_leftClickPos;
	bool m_doubleClick;

	QMap<QByteArray, CItem*> m_itemFactories;
	CItem *m_activeItemFactory;

	QMap<QByteArray, QByteArray> m_classToSuperIds;

	IUndoManager *m_undoManager;

	ClassAttributesMap m_classAttributes;
    QMap<QByteArray, QSet<QByteArray>> m_classAttributesVis;

private:
    int m_gridSize;
    bool m_gridEnabled;
    bool m_gridSnap;
    QPen m_gridPen;
	bool m_labelsEnabled, m_labelsUpdate;

	QSet<CItem*> m_acceptedHovers, m_rejectedHovers;

	QGraphicsItem *m_menuTriggerItem;
	QGraphicsItem *m_draggedItem;

	QCursor m_sceneCursor;

	// paint speedups
	QPainterPath m_usedLabelsRegion;
};


// factorization

template<class T>
T* CEditorScene::createItemOfType(QPointF* at) const
{
	CItem* item = createItemOfType(T::factoryId());
	if (item)
	{
		T* titem = dynamic_cast<T*>(item);
		if (titem)
		{
			if (at)
			{
				(const_cast<CEditorScene*>(this))->addItem(titem);
				titem->setPos(*at);
			}

			return titem;
		}

		delete item;
		return NULL;
	}

	return NULL;
}


// selections

template<class T, class L = T>
QList<L*> CEditorScene::getSelectedItems(bool triggeredIfEmpty) const
{
	QList<L*> result;

	auto selItems = selectedItems();
	if (selItems.isEmpty() && triggeredIfEmpty && m_menuTriggerItem)
		selItems.append(m_menuTriggerItem);

	for (auto* item : selItems)
	{
		T* titem = dynamic_cast<T*>(item);
		if (titem)
			result.append(titem);
	}

	return result;
}


template<class T, class L = T>
QList<L*> CEditorScene::getItems() const
{
	QList<L*> result;

	for (auto* item : items())
	{
		T* titem = dynamic_cast<T*>(item);
		if (titem)
			result.append(titem);
	}

	return result;
}


#endif // CEDITORSCENE_H

#include "CEditorScene.h"
#include "CItem.h"
#include "CSimpleUndoManager.h"

#include <QPainter>
#include <QPaintEngine>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QInputDialog>

#include <qopengl.h>


const quint64 version64 = 5;	// build
const char* versionId = "VersionId";


CEditorScene::CEditorScene(QObject *parent): QGraphicsScene(parent), 
	m_menuTriggerItem(NULL),
	m_draggedItem(NULL),
	m_activeItemFactory(NULL),
	m_undoManager(new CSimpleUndoManager(*this)),
	m_doubleClick(false)
{
    m_gridSize = 25;
    m_gridEnabled = true;
    m_gridSnap = false;
    m_gridPen = QPen(Qt::gray, 0, Qt::DotLine);

	m_labelsEnabled = true;
	m_labelsUpdate = false;

    //setSceneRect(-1000, -1000, 2000, 2000);

	setItemIndexMethod(QGraphicsScene::NoIndex);

	// init scene 
	activate();
}

CEditorScene::~CEditorScene()
{
	reset();
}

void CEditorScene::reset()
{
	initialize();

	m_undoManager->reset();

	setSceneRect(QRectF(-500,-500,1000,1000));
}

void CEditorScene::initialize()
{
	removeItems();

	m_classAttributes.clear();
	m_classAttributesVis.clear();

	// default item attr
    CAttribute labelAttr("item", "label", "Label", "");
    setClassAttribute(labelAttr, true);

    CAttribute idAttr("item", "id", "ID", "");
    setClassAttribute(idAttr, true);
}

void CEditorScene::removeItems()
{
	while (!items().isEmpty())
		delete items().first();

	clear();
}

void CEditorScene::activate()
{
	addUndoState();
}

// properties

void CEditorScene::setGridSize(int newSize)
{
    if (newSize <= 0)
        return;

    m_gridSize = newSize;

    update();
}

void CEditorScene::enableGrid(bool on)
{
    m_gridEnabled = on;

    update();
}

void CEditorScene::enableGridSnap(bool on)
{
    m_gridSnap = on;
}

void CEditorScene::setGridPen(const QPen &gridPen)
{
    m_gridPen = gridPen;

    update();
}

void CEditorScene::enableItemLabels(bool on)
{
	m_labelsEnabled = on;

	update();
}

// undo-redo

void CEditorScene::undo()
{
	if (m_undoManager)
	{
		m_undoManager->undo();

		checkUndoState();

		Q_EMIT sceneChanged();
	}
}

void CEditorScene::redo()
{
	if (m_undoManager)
	{
		m_undoManager->redo();

		checkUndoState();

		Q_EMIT sceneChanged();
	}
}

void CEditorScene::addUndoState()
{
	// canvas size
	QRectF minRect(-500, -500, 1000, 1000);
	minRect |= itemsBoundingRect().adjusted(-20, -20, 20, 20);
	setSceneRect(minRect);

	// undo-redo
	if (m_undoManager)
	{
		m_undoManager->addState();

		checkUndoState();
	}

	// notification
	Q_EMIT sceneChanged();
}

int CEditorScene::availableUndoCount() const 
{ 
	return m_undoManager ? m_undoManager->availableUndoCount() : 0; 
}

int CEditorScene::availableRedoCount() const 
{ 
	return m_undoManager ? m_undoManager->availableRedoCount() : 0; 
}

void CEditorScene::checkUndoState()
{
	Q_EMIT undoAvailable(m_undoManager->availableUndoCount() > 0);
	Q_EMIT redoAvailable(m_undoManager->availableRedoCount() > 0);
}

// io

bool CEditorScene::storeTo(QDataStream& out) const
{
    out << versionId << version64;

	// items
	QMap<CItem*, uint> sortedMap;

	QList<QGraphicsItem*> allItems = items();

	for(QGraphicsItem* item : allItems)
	{
		CItem* citem = dynamic_cast<CItem*>(item);
		if (citem)
		{
            sortedMap[citem] = quint64(citem);
			//out << citem->typeId() << quintptr(citem);

			//citem->storeTo(out);
		}
	}

	for (CItem* citem : sortedMap.keys())
	{
        out << citem->typeId() << quint64(citem);

		citem->storeTo(out, version64);
	}

	// attributes
	out << QByteArray("_attr_");
	out << (quint64)0x12345678;

	out << m_classAttributes.size();
	for (auto classAttrs : m_classAttributes)
	{
		out << classAttrs.size();
		for (auto attr : classAttrs)
		{
			attr.storeTo(out, version64);
		}
	}

	out << m_classToSuperIds;

	// visible attributes
	out << m_classAttributesVis;

	return true;
}

bool CEditorScene::restoreFrom(QDataStream& out)
{
	initialize();

	// version
	quint64 storedVersion = 0;

	// read
	CItem::CItemLinkMap idToItem;

	while (!out.atEnd())
	{
		QByteArray id; out >> id;
        quint64 ptrId; out >> ptrId;

		if (storedVersion == 0 && strcmp(id.data(), versionId) == 0)
		{
			storedVersion = ptrId;
			out >> id >> ptrId;
		}

		// started attr section
		if (storedVersion >= 3 && id == "_attr_" && ptrId == 0x12345678)
			break;

		CItem* item = createItemOfType(id);
		if (item)
		{
			if (item->restoreFrom(out, storedVersion))
			{
                idToItem[ptrId] = item;
				continue;
			}
		}

		// failed: cleanup
		qDeleteAll(idToItem.values());

		return false;
	}

	// link items
	for (CItem* item : idToItem.values())
	{
		if (item->linkAfterRestore(idToItem))
		{
			addItem(dynamic_cast<QGraphicsItem*>(item));
		}
		else
		{
			// failed: cleanup
			qDeleteAll(idToItem.values());

			clear();

			return false;
		}
	}

	// attributes
	if (version64 >= 3)
	{
		int classAttrSize = 0;
		out >> classAttrSize;

		for (int i = 0; i < classAttrSize; ++i)
		{
			int attrSize = 0;
			out >> attrSize;

			for (int j = 0; j < attrSize; ++j)
			{
				CAttribute attr;
				if (attr.restoreFrom(out, version64))
					setClassAttribute(attr);
				else
					return false;
			}
		}
	}

	// visible attributes
	if (version64 >= 5)
	{
		out >> m_classToSuperIds;
		out >> m_classAttributesVis;
	}

	return true;
}

// factorization

bool CEditorScene::addItemFactory(CItem *factoryItem)
{
	if (factoryItem)
	{
		// register class inheritance
		QByteArray classId = factoryItem->classId();
		QByteArray superClassId = factoryItem->superClassId();
		m_classToSuperIds[classId] = superClassId;

		QByteArray id = factoryItem->typeId();

		// already registered?
		if (m_itemFactories.contains(id))
		{
			return (m_itemFactories[id] == factoryItem);
		}

		m_itemFactories[id] = factoryItem;
		return true;
	}

	return false;
}

CItem* CEditorScene::activateItemFactory(const QByteArray &factoryId)
{
	if (factoryId.isEmpty() || !m_itemFactories.contains(factoryId))
	{
		m_activeItemFactory = NULL;
	}
	else
	{
		m_activeItemFactory = m_itemFactories[factoryId];
	}

	return NULL;
}

CItem* CEditorScene::createItemOfType(const QByteArray &id) const
{
	if (m_itemFactories.contains(id))
	{
		return m_itemFactories[id]->create();
	}

	return NULL;
}

// attributes

void CEditorScene::setClassAttribute(const CAttribute& attr, bool vis) 
{
	m_classAttributes[attr.classId][attr.id] = attr;

	setClassAttributeVisible(attr.classId, attr.id, vis);
}

void CEditorScene::setClassAttributeVisible(const QByteArray& classId, const QByteArray& attrId, bool vis)
{
	if (vis)
		m_classAttributesVis[classId].insert(attrId);
	else
		m_classAttributesVis[classId].remove(attrId);

	// set label update flag
	m_labelsUpdate = true;

	// schedule update
	invalidate();
}

QSet<QByteArray> CEditorScene::getVisibleClassAttributes(const QByteArray& classId, bool inherited) const
{
	QSet<QByteArray> result = m_classAttributesVis[classId];

	if (inherited)
	{
		QByteArray superId = getSuperClassId(classId);
		while (!superId.isEmpty())
		{
			result = result.unite(m_classAttributesVis[superId]);
			superId = getSuperClassId(superId);
		}
	}

	return result;
}

AttributesMap CEditorScene::getClassAttributes(const QByteArray& classId, bool inherited) const
{
	AttributesMap result = m_classAttributes[classId];

	if (inherited)
	{
		QByteArray superId = getSuperClassId(classId);
		while (!superId.isEmpty())
		{
			result = result.unite(m_classAttributes[superId]);
			superId = getSuperClassId(superId);
		}
	}

	return result;
}

// callbacks

void CEditorScene::onItemDestroyed(CItem *citem)
{
	Q_ASSERT(citem);
}

// protected

void CEditorScene::drawBackground(QPainter *painter, const QRectF &)
{
	// reset region
	m_usedLabelsRegion = QPainterPath();

	if (painter->paintEngine()->type() == QPaintEngine::OpenGL || painter->paintEngine()->type() == QPaintEngine::OpenGL2)
	{
		glClearColor(1, 1, 1, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	painter->setPen(QPen(Qt::darkGray, 2, Qt::SolidLine));
	QColor fillColor("lightsteelblue");
	fillColor.setAlpha(50);
	painter->setBrush(fillColor);
	painter->drawRect(sceneRect());

	if (m_gridSize < 0 || !m_gridEnabled)
		return;

	painter->setPen(m_gridPen);

	QRectF rect = sceneRect();

	qreal left = int(rect.left()) - (int(rect.left()) % m_gridSize);
	qreal top = int(rect.top()) - (int(rect.top()) % m_gridSize);

	QVarLengthArray<QLineF, 100> lines;

	for (qreal x = left; x < rect.right(); x += m_gridSize)
		lines.append(QLineF(x, rect.top(), x, rect.bottom()));
	for (qreal y = top; y < rect.bottom(); y += m_gridSize)
		lines.append(QLineF(rect.left(), y, rect.right(), y));

	//qDebug() << lines.size();

	painter->drawLines(lines.data(), lines.size());
}

void CEditorScene::drawForeground(QPainter *painter, const QRectF &r)
{
	Super::drawForeground(painter, r);

	// drop label update flag
	m_labelsUpdate = false;
}

bool CEditorScene::checkLabelRegion(const QRectF &r)
{
	if (m_usedLabelsRegion.intersects(r))
		return false;

	m_usedLabelsRegion.addRect(r);
	return true;
}

// mousing

void CEditorScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	// workaround: do not deselect selected items by RMB
	if (mouseEvent->button() != Qt::LeftButton)
	{
		mouseEvent->accept();
		return;
	}

	Super::mousePressEvent(mouseEvent);

	if (mouseEvent->button() == Qt::LeftButton)
	{
		m_draggedItem = NULL;

		m_leftClickPos = mouseEvent->scenePos();
	}
}

void CEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	Super::mouseMoveEvent(mouseEvent);

	m_draggedItem = mouseGrabberItem();

	moveDrag(mouseEvent, m_draggedItem);
}

void CEditorScene::moveDrag(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem* dragItem)
{
	m_leftClickPos = QPointF();
	m_doubleClick = false;

	if (dragItem)
	{
		if (dragItem->flags() & dragItem->ItemIsMovable)
		{
			QSet<CItem*> oldHovers = m_acceptedHovers + m_rejectedHovers;

			QList<QGraphicsItem*> hoveredItems = dragItem->collidingItems();
			for (int i = 0; i < hoveredItems.size(); ++i)
			{
				// dont drop on disabled
				if (!hoveredItems.at(i)->isEnabled())
					continue;

				// if many items being dragged: dont drop on selected
				if (hoveredItems.at(i)->isSelected())
					continue;

				CItem* item = dynamic_cast<CItem*>(hoveredItems.at(i));
				if (item)
				{
					oldHovers.remove(item);
					if (m_acceptedHovers.contains(item) || m_rejectedHovers.contains(item))
						continue;

					ItemDragTestResult result = item->acceptDragFromItem(dragItem);

					if (result == Accepted)
					{
						m_acceptedHovers.insert(item);

						item->setItemStateFlag(IS_Drag_Accepted);
						item->resetItemStateFlag(IS_Drag_Rejected);
					}
					else if (result == Rejected)
					{
						m_rejectedHovers.insert(item);

						item->resetItemStateFlag(IS_Drag_Accepted);
						item->setItemStateFlag(IS_Drag_Rejected);
					}
				}
			}

			// deactivate left hovers
			for (CItem* item : oldHovers)
			{
				item->leaveDragFromItem(dragItem);

				m_acceptedHovers.remove(item);
				m_rejectedHovers.remove(item);

				item->resetItemStateFlag(IS_Drag_Accepted);
				item->resetItemStateFlag(IS_Drag_Rejected);
			}

			// inform the dragger
			CItem* draggedCItem = dynamic_cast<CItem*>(dragItem);
			if (draggedCItem)
			{
				draggedCItem->onDraggedOver(m_acceptedHovers, m_rejectedHovers);
			}

			// inform the scene
			onDragging(dragItem, m_acceptedHovers, m_rejectedHovers);
		}
	}
	else	// no drag, just hover
	{	
		QGraphicsItem *hoverItem = itemAt(mouseEvent->scenePos(), QTransform());

		// inform the scene
		onMoving(mouseEvent, hoverItem);
	}

	updateSceneCursor();
}

void CEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	Super::mouseDoubleClickEvent(mouseEvent);

	if (mouseEvent->button() == Qt::LeftButton)
	{
		m_doubleClick = true;
	}

	updateSceneCursor();
}

void CEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsItem* prevGrabber = m_draggedItem;

	Super::mouseReleaseEvent(mouseEvent);

	m_draggedItem = mouseGrabberItem();

	if (mouseEvent->button() == Qt::LeftButton)
	{
		finishDrag(mouseEvent, prevGrabber, false);

		if (m_leftClickPos == mouseEvent->scenePos())
		{
			QGraphicsItem* itemUnderMouse = itemAt(m_leftClickPos, QTransform());

			if (m_doubleClick)
				onLeftDoubleClick(mouseEvent, itemUnderMouse);
			else
				onLeftClick(mouseEvent, itemUnderMouse);
		}

		m_doubleClick = false;
	}
}

void CEditorScene::finishDrag(QGraphicsSceneMouseEvent* /*mouseEvent*/, QGraphicsItem* dragItem, bool dragCancelled)
{
	if (dragItem)
	{
		// deactivate left hovers
		for (CItem* item : m_acceptedHovers)
		{
			item->leaveDragFromItem(dragItem);

			item->resetItemStateFlag(IS_Drag_Accepted);
			item->resetItemStateFlag(IS_Drag_Rejected);
		}

		for (CItem* item : m_rejectedHovers)
		{
			item->leaveDragFromItem(dragItem);

			item->resetItemStateFlag(IS_Drag_Accepted);
			item->resetItemStateFlag(IS_Drag_Rejected);
		}

		// inform the dragger
		CItem* draggedItem = dynamic_cast<CItem*>(dragItem);
		if (draggedItem && !dragCancelled)
		{
			draggedItem->onDroppedOn(m_acceptedHovers, m_rejectedHovers);
		}

		// drag finish
		m_acceptedHovers.clear();
		m_rejectedHovers.clear();

		// update undo manager
		if (!dragCancelled)
		{
			addUndoState();
		}
	}

	setSceneCursor(Qt::ArrowCursor);

	updateSceneCursor();
}

void CEditorScene::onMoving(QGraphicsSceneMouseEvent *mouseEvent, QGraphicsItem* hoverItem)
{
	if (hoverItem)
	{
		if (hoverItem->isEnabled() && (hoverItem->flags() & hoverItem->ItemIsMovable))
		{
			if (mouseEvent->buttons() == Qt::NoButton)
			{
				setSceneCursor(Qt::SizeAllCursor);
				return;
			}
		}
	}

	setSceneCursor(Qt::ArrowCursor);
}

void CEditorScene::onDragging(QGraphicsItem* /*dragItem*/, const QSet<CItem*>& acceptedItems, const QSet<CItem*>& rejectedItems)
{
	if (acceptedItems.size())
	{
		setSceneCursor(Qt::CrossCursor);
		return;
	}

	if (rejectedItems.size())
	{
		setSceneCursor(Qt::ForbiddenCursor);
		return;
	}

	setSceneCursor(Qt::SizeAllCursor);
}

void CEditorScene::onLeftDoubleClick(QGraphicsSceneMouseEvent* /*mouseEvent*/, QGraphicsItem* clickedItem)
{
	if (CItem *item = dynamic_cast<CItem*>(clickedItem))
	{
		bool ok = false;

		QString text = QInputDialog::getMultiLineText(NULL,
			tr("Item Label"), tr("New label text:"), 
			item->getAttribute("label").toString(),
			&ok);

		if (ok)
		{
			item->setAttribute("label", text);

			addUndoState();
		}
	}
}

// private

void CEditorScene::updateSceneCursor()
{
	for (QGraphicsView* v : views())
	{
		v->setCursor(m_sceneCursor);
	}
}

// keys

void CEditorScene::keyPressEvent(QKeyEvent *keyEvent)
{
	Super::keyPressEvent(keyEvent);
	if (keyEvent->isAccepted())
		return;

	if (keyEvent->key() == Qt::Key_Delete)
	{
		onActionDelete();

		keyEvent->accept();
		return;
	}

	if (keyEvent->key() == Qt::Key_A && keyEvent->modifiers() == Qt::ControlModifier)
	{
		onActionSelectAll();

		keyEvent->accept();
		return;
	}
}

// menu stuff 

void CEditorScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
	QMenu sceneMenu;

	m_menuTriggerItem = itemAt(contextMenuEvent->scenePos(), QTransform()); //Get the item at the position
	
	if (populateMenu(sceneMenu, m_menuTriggerItem, selectedItems()))
	{
		sceneMenu.exec(contextMenuEvent->screenPos());
	}
}

bool CEditorScene::populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems)
{
	if (!item && selectedItems.isEmpty())
		return false;

	// add default actions
	QAction *deleteAction = menu.addAction(tr("Delete"), this, SLOT(onActionDelete()));
	deleteAction->setEnabled(createSelectedList(CDeletableItems()).size());

	return true;
}

void CEditorScene::onActionDelete()
{
	QList<QGraphicsItem*> itemList = createSelectedList(CDeletableItems());
	if (itemList.isEmpty())
		return;

	if (QMessageBox::question(NULL, tr("Delete Items"), tr("You are about to delete %1 item(s). Sure?").arg(itemList.size())) == QMessageBox::No)
		return;

	for (QGraphicsItem* item : itemList)
	{
		if (items().contains(item))
			delete item;
	}

	addUndoState();
}

void CEditorScene::onActionSelectAll()
{
	blockSignals(true);

	QList<QGraphicsItem*> itemList = items();

	for (QGraphicsItem* item : itemList)
	{
		item->setSelected(true);
	}

	blockSignals(false);

	Q_EMIT selectionChanged();
}

// evaluators

QList<QGraphicsItem*> CEditorScene::createSelectedList(const CItemsEvaluator& eval) const
{
	QList<QGraphicsItem*> result;

	QList<QGraphicsItem*> itemList = selectedItems();
	if (itemList.isEmpty() && m_menuTriggerItem)
		itemList.append(m_menuTriggerItem);

	for (int i = 0; i < itemList.size(); ++i)
	{
		if (eval.evaluate(*itemList.at(i)))
			result << itemList.at(i);
	}

	return result;
}

bool CDeletableItems::evaluate(const QGraphicsItem& item) const
{
	const CItem* citem = dynamic_cast<const CItem*>(&item);
	if (citem)
	{
		return citem->itemFlags() & IF_DeleteAllowed;
	}

	// can delete QGraphicsItem
	return true;
}

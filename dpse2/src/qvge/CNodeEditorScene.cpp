#include "CNodeEditorScene.h"
#include "CNode.h"
#include "CNodePort.h"
#include "CEdge.h"
#include "CDirectEdge.h"
#include "CControlPoint.h"
#include "CEditorSceneDefines.h"

#include <QGraphicsSceneMouseEvent>
#include <QColorDialog> 
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>


CNodeEditorScene::CNodeEditorScene(QObject *parent) : Super(parent),
	m_editMode(EM_Default),
	m_startNode(NULL),
	m_endNode(NULL),
	m_connection(NULL),
	m_realStart(false),
	m_state(IS_None)
{
	// default factories
	registerItemFactory<CDirectEdge>();
	registerItemFactory<CNode>();

	// go
	initialize();
}


// reimp

void CNodeEditorScene::initialize()
{
	Super::initialize();


	// common constrains
	static CAttributeConstrainsList *edgeStyles = new CAttributeConstrainsList();
	if (edgeStyles->ids.isEmpty()) {
		edgeStyles->names << "None" << "Solid" << "Dots" << "Dashes" << "Dash-Dot" << "Dash-Dot-Dot";
		edgeStyles->ids << "none" << "solid" << "dotted" << "dashed" << "dashdot" << "dashdotdot";
	}


	// default node attr
    CAttribute nodeAttr("color", "Color", QColor(Qt::magenta));
	setClassAttribute("node", nodeAttr);

    CAttribute shapeAttr("shape", "Shape", "disc");
	setClassAttribute("node", shapeAttr);

	//CAttribute sizeAttr("size", "Size", 11.0);
	//setClassAttribute("node", sizeAttr);
	//setClassAttributeConstrains("node", "size", new CDoubleConstrains(0.1, 1000.0));
	//createClassAttribute("node", "size", "Size", 11.0, new CDoubleConstrains(0.1, 1000.0));
	createClassAttribute("node", "size", "Size", QSizeF(11.0, 11.0));

	createClassAttribute("node", "stroke.style", "Stroke Style", "solid");
	setClassAttributeConstrains("node", "stroke.style", edgeStyles);
	createClassAttribute("node", "stroke.size", "Stroke Size", 1.0);
	createClassAttribute("node", "stroke.color", "Stroke Color", QColor(Qt::black));

    CAttribute posAttr("pos", "Position", QPointF());
	posAttr.noDefault = true;
	setClassAttribute("node", posAttr);

	// default edge attr
    CAttribute edgeAttr("color", "Color", QColor(Qt::gray));
	setClassAttribute("edge", edgeAttr);

	CAttribute directionAttr("direction", "Direction", "directed");
	setClassAttribute("edge", directionAttr);

	CAttribute weightAttr("weight", "Weight", 1.0);
	setClassAttribute("edge", weightAttr);

    CAttribute styleAttr("style", "Style", "solid");
    setClassAttribute("edge", styleAttr);


	static CAttributeConstrainsList *edgeDirections = new CAttributeConstrainsList();
	if (edgeDirections->ids.isEmpty()) {
		edgeDirections->names << tr("Directed (one end)") << tr("Mutual (both ends)") << tr("None (no ends)");
		edgeDirections->ids << "directed" << "mutual" << "undirected";
		edgeDirections->icons << QIcon(":/Icons/Edge-Directed") << QIcon(":/Icons/Edge-Mutual") << QIcon(":/Icons/Edge-Undirected");
	}
	setClassAttributeConstrains("edge", "direction", edgeDirections);

	setClassAttributeConstrains("edge", "style", edgeStyles);

	static CAttributeConstrainsList *nodeShapes = new CAttributeConstrainsList();
	if (nodeShapes->ids.isEmpty()) {
		nodeShapes->names << tr("Dics") << tr("Square") << tr("Triangle (up)") << tr("Triangle (down)") << tr("Diamond") << tr("Hexagon");
		nodeShapes->ids << "disc" << "square" << "triangle" << "triangle2" << "diamond" << "hexagon";
		nodeShapes->icons << QIcon(":/Icons/Node-Disc") << QIcon(":/Icons/Node-Square") << QIcon(":/Icons/Node-Triangle") 
			<< QIcon(":/Icons/Node-Triangle-Down") << QIcon(":/Icons/Node-Diamond") << QIcon(":/Icons/Node-Hexagon");
	}
	setClassAttributeConstrains("node", "shape", nodeShapes);
}


void CNodeEditorScene::initializeOnce()
{
	Super::initializeOnce();
}


// nodes creation

void CNodeEditorScene::setEditMode(EditMode mode)
{
	if (m_editMode != mode)
	{
		m_editMode = mode;

		Q_EMIT editModeChanged(m_editMode);
	}
}


bool CNodeEditorScene::startNewConnection(const QPointF& pos)
{
	if (QGraphicsItem* item = getItemAt(pos))
	{
		if (!item->isEnabled())
			return false;

		// check for port first
		CNodePort *port = dynamic_cast<CNodePort*>(item);
		if (port)
		{
			CNode *node = port->getNode();
			Q_ASSERT(node != NULL);

			if (!node->allowStartConnection())
				return false;

			m_realStart = false;
			m_startNode = node;
			m_startNodePort = port;
		}
		else
		{
			// check for node
			CNode *node = dynamic_cast<CNode*>(item);
			if (!node)
				return false;

			if (!node->allowStartConnection())
				return false;

			m_realStart = false;
			m_startNode = node;
			m_startNodePort = NULL;
		}
	}
	else
	{
		m_realStart = true;
		m_startNode = createNewNode(getSnapped(pos));
		m_startNodePort = NULL;
	}

	//m_endNode = dynamic_cast<CNode*>(m_startNode->clone());
	//addItem(m_endNode);
	//m_endNode->setPos(getSnapped(pos));

	m_endNode = createNewNode(getSnapped(pos));

	Super::startDrag(m_endNode);

	m_connection = createNewConnection(m_startNode, m_endNode);

	if (m_startNodePort)
		m_connection->setFirstNode(m_startNode, m_startNodePort->getId());

	m_state = IS_Creating;

    // auto select created items
    m_startNode->setSelected(false);
    //m_connection->setSelected(true);
    m_endNode->setSelected(true);

	return true;
}


void CNodeEditorScene::cancel(const QPointF& /*pos*/)
{
	// if not cancelling already
	if (m_state != IS_Cancelling)
	{
		// cancel current drag operation
		Super::finishDrag(NULL, m_startDragItem, true);

		// if no creating state: return
		if (m_state != IS_Creating)
		{
			m_state = IS_None;
			return;
		}
	}

	m_state = IS_None;

	// kill connector
	m_connection->setFirstNode(NULL);
	m_connection->setLastNode(NULL);
	delete m_connection;
	m_connection = NULL;

	// kill end
	delete m_endNode;
	m_endNode = NULL;

	// kill start if real
	if (m_realStart)
		delete m_startNode;

	m_startNode = NULL;
	m_realStart = false;
}


CNode* CNodeEditorScene::createNewNode() const
{
	auto nodeFactory = getActiveItemFactory("CNode");
	if (nodeFactory)
	{
		auto node = dynamic_cast<CNode*>(nodeFactory->create());
		Q_ASSERT(node);
		node->copyDataFrom(nodeFactory);
		return node;
	}

	// here default
	return new CNode;
}


CNode* CNodeEditorScene::createNewNode(const QPointF& pos)
{
	auto node = createNewNode();
	addItem(node);
	node->setPos(pos);
	return node;
}


CEdge* CNodeEditorScene::createNewConnection() const
{
	auto edgeFactory = getActiveItemFactory("CDirectEdge");
	if (edgeFactory)
	{
		auto edge = dynamic_cast<CDirectEdge*>(edgeFactory->create());
		Q_ASSERT(edge);
		edge->copyDataFrom(edgeFactory);
		return edge;
	}

	// here default
	return new CDirectEdge();
}


CEdge* CNodeEditorScene::createNewConnection(CNode* startNode, CNode* endNode)
{
	auto edge = createNewConnection();
	addItem(edge);
	edge->setFirstNode(startNode);
	edge->setLastNode(endNode);
	return edge;
}


// events

void CNodeEditorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//if (m_state == IS_None)
	if (m_startDragItem == NULL)
	{
		// call super
 		Super::mouseReleaseEvent(mouseEvent);
		return;
	}

	// release local grabber if any
	if (m_state == IS_Creating)
	{
		m_state = IS_Finishing;

		// cancel on RMB
		if (mouseEvent->button() == Qt::RightButton)
		{
			m_state = IS_Cancelling;
		}

		// cancel on same position
		if (m_startNode->pos() == m_endNode->pos())
		{
			m_state = IS_Cancelling;
		}
	}

	// call super
	finishDrag(mouseEvent, m_startDragItem, m_state == IS_Cancelling);

	// finish
	if (m_state == IS_Cancelling)
	{
		cancel(mouseEvent->scenePos());
	}
	else
	if (m_state == IS_Finishing)
	{
		//m_connection->setSelected(true);
	}

	m_state = IS_None;

	// necessary to handle scene events properly
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


void CNodeEditorScene::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->key() == Qt::Key_Escape)
	{
		cancel();
		return;
	}

	Super::keyPressEvent(keyEvent);
}


// handlers

void CNodeEditorScene::onLeftButtonPressed(QGraphicsSceneMouseEvent *mouseEvent)
{
	Super::onLeftButtonPressed(mouseEvent);

	// add nodes?
	if (m_editMode == EM_AddNodes || isItemAt<CNodePort>(mouseEvent->scenePos()))
	{
		deselectAll();

		// skip calling super to avoid auto selection
		mouseEvent->accept();
	}
}


bool CNodeEditorScene::onClickDrag(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &clickPos)
{
	// add nodes?
	if ((m_editMode == EM_AddNodes) || isItemAt<CNodePort>(clickPos))
	{
		if (startNewConnection(clickPos))
		{
			setEditMode(EM_Default);
			return true;
		}
	}

	// else super
	return Super::onClickDrag(mouseEvent, clickPos);
}


bool CNodeEditorScene::onDoubleClickDrag(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &clickPos)
{
	// debug
	if (clickPos == QPointF())
		qDebug() << "bug";

	// try to start new connection at click point
	if (startNewConnection(clickPos))
	{
		//mouseEvent->accept();
		return true;
	}

	// else call super
	return Super::onDoubleClickDrag(mouseEvent, clickPos);
}


void CNodeEditorScene::onDropped(QGraphicsSceneMouseEvent* mouseEvent, QGraphicsItem* dragItem)
{
	CNode *dragNode = dynamic_cast<CNode*>(dragItem);

	// perform snap
	if (gridSnapEnabled())
	{
		// control point:
		if (auto cp = dynamic_cast<CControlPoint*>(dragItem))
		{
			auto newPos = getSnapped(cp->scenePos());
			cp->setPos(newPos);
			return;
		}

		// nodes & edges:
		QSet<QGraphicsItem*> items;
		QSet<CEdge*> edges;

		if (!dragNode) 
		{
			if (auto edge = dynamic_cast<CEdge*>(dragItem))
			{
				edges << edge;

				dragNode = edge->firstNode();
			}
		}

		if (dragNode)
		{
			items << dragNode;

			auto newPos = getSnapped(dragNode->scenePos());
			auto d = newPos - dragNode->scenePos();

			for (auto item : selectedItems())
			{
				if (auto edge = dynamic_cast<CEdge*>(item))
				{
					edges << edge;
					items << edge->firstNode();
					items << edge->lastNode();
				}
				else
					items << item;
			}

			for (auto item : items)
				item->moveBy(d.x(), d.y());

			for (auto edge : edges)
				edge->onItemMoved(d);

			return;
		}

		// whatever:
	}

	Super::onDropped(mouseEvent, dragItem);
}


void CNodeEditorScene::onLeftClick(QGraphicsSceneMouseEvent* mouseEvent, QGraphicsItem* clickedItem)
{
	if (m_editMode == EM_AddNodes)
	{
		// clicked on empty space?
		if (!clickedItem)
		{
			onLeftDoubleClick(mouseEvent, clickedItem);
			setEditMode(EM_Default);
			return;
		}
	}

	Super::onLeftClick(mouseEvent, clickedItem);
}


void CNodeEditorScene::onLeftDoubleClick(QGraphicsSceneMouseEvent* mouseEvent, QGraphicsItem* clickedItem)
{
	// clicked on empty space?
	if (!clickedItem)
	{
		// create a node here
		auto node = createNewNode(getSnapped(mouseEvent->scenePos()));
		node->setSelected(true);

		addUndoState();
		return;
	}

	Super::onLeftDoubleClick(mouseEvent, clickedItem);
}


// movement

void CNodeEditorScene::moveSelectedEdgesBy(const QPointF& d)
{
	QList<CEdge*> edges = getSelectedItems<CEdge>();
	if (edges.size())
	{
		QSet<CNode*> unselNodes;	// not selected nodes

									// move selected edges
		for (auto edge : edges)
		{
			if (!edge->firstNode()->isSelected())
				unselNodes << edge->firstNode();

			if (!edge->lastNode()->isSelected())
				unselNodes << edge->lastNode();

			edge->onItemMoved(d);
		}

		// force move non selected nodes of the selected edges
		for (auto node : unselNodes)
		{
			node->moveBy(d.x(), d.y());
		}
	}
}


// reimp

void CNodeEditorScene::moveSelectedItemsBy(const QPointF& d)
{
	QSet<QGraphicsItem*> items;
	QSet<CEdge*> edges;

	for (auto item : selectedItems())
	{
		if (!(item->flags() & item->ItemIsMovable))
			continue;
		else
		if (auto edge = dynamic_cast<CEdge*>(item))
		{
			edges << edge;
			items << edge->firstNode();
			items << edge->lastNode();
		}
		else
			items << item; 
	}

	for (auto item : items)
		item->moveBy(d.x(), d.y());

	for (auto edge : edges)
		edge->onItemMoved(d);
}


QList<QGraphicsItem*> CNodeEditorScene::copyPasteItems() const
{
	// only selected edges & their nodes
	QList<QGraphicsItem*> result;

	QSet<QGraphicsItem*> nodes;

	for (auto item: selectedItems())
	{
		if (auto edge = dynamic_cast<CEdge*>(item))
		{
			result << edge;
			nodes << edge->firstNode();
			nodes << edge->lastNode();
		}
		else
		if (auto node = dynamic_cast<CNode*>(item))
		{
			// orphaned nodes only
			if (node->nodeFlags() & NF_OrphanAllowed)
				nodes << node;
		}
		else
			result << item;
	}

	result << nodes.toList();

    return result;
}


QList<QGraphicsItem*> CNodeEditorScene::transformableItems() const
{
	QList<QGraphicsItem*> result;
	
	auto nodes = getSelectedNodes();
	for (auto node : nodes)
		result << node;

	return result;
}


bool CNodeEditorScene::doUpdateCursorState(Qt::KeyboardModifiers keys, Qt::MouseButtons buttons, QGraphicsItem *hoverItem)
{
	// handled by super?
	if (Super::doUpdateCursorState(keys, buttons, hoverItem))
		return true;

	if (CNodePort *portItem = dynamic_cast<CNodePort*>(hoverItem))
	{
		if (portItem->isEnabled())
		{
			setSceneCursor(Qt::UpArrowCursor);
			setInfoStatus(SIS_Hover_Port);
			return true;
		}
	}

	// still not handled
	return false;
}


// painting

void CNodeEditorScene::drawBackground(QPainter *painter, const QRectF &r)
{
    Super::drawBackground(painter, r);
}


void CNodeEditorScene::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[],
                                 const QStyleOptionGraphicsItem options[],
                                 QWidget *widget)
{
    QElapsedTimer tm;
    tm.start();

    static int maxtime = 0;

    // test only
//    Super::drawItems(painter, numItems, items, options, widget);

    for (int i = m_nextIndex; i < numItems; ++i)
    {
        // Draw the item
        painter->save();
        painter->setTransform(items[i]->sceneTransform(), true);
        items[i]->paint(painter, &options[i], widget);
        painter->restore();

//        if (tm.elapsed() > 50)
//        {
//            m_nextIndex = i+1;
//            update();
//            return;
//        }
    }

    m_nextIndex = 0;

    if (tm.elapsed() > maxtime)
    {
        maxtime = tm.elapsed();
        qDebug() << tm.elapsed();
    }

}



// selections

const QList<CNode*>& CNodeEditorScene::getSelectedNodes() const
{
    if (m_selNodes.isEmpty())
        prefetchSelection();

    return m_selNodes;
}


const QList<CEdge*>& CNodeEditorScene::getSelectedEdges() const
{
    if (m_selEdges.isEmpty())
        prefetchSelection();

    return m_selEdges;
}


void CNodeEditorScene::onSelectionChanged()
{
    // drop cached selections
    m_selNodes.clear();
    m_selEdges.clear();

	Super::onSelectionChanged();
}


void CNodeEditorScene::prefetchSelection() const
{
    m_selNodes.clear();
    m_selEdges.clear();

    auto selItems = selectedItems();

    for (auto* item : selItems)
    {
        if (CNode* node = dynamic_cast<CNode*>(item))
        {
            m_selNodes << node;
            continue;
        }

        if (CEdge* edge = dynamic_cast<CEdge*>(item))
        {
            m_selEdges << edge;
            continue;
        }
    }
}


// menu & actions

void CNodeEditorScene::onActionLink()
{
	QList<CNode*> nodes = getSelectedItems<CNode>(true);
	if (nodes.count() < 2)
		return;

	auto baseNode = nodes.takeFirst();
	for (auto node : nodes)
	{
		baseNode->merge(node);
	}

	addUndoState();
}


void CNodeEditorScene::onActionUnlink()
{
    QList<CNode*> nodes = getSelectedItems<CNode>(true);
    if (nodes.isEmpty())
        return;

    for (auto node : nodes)
    {
        node->unlink();
    }

    addUndoState();
}


void CNodeEditorScene::onActionNodeColor()
{
    QList<CNode*> nodes = getSelectedItems<CNode>(true);
    if (nodes.isEmpty())
        return;

    QColor color = QColorDialog::getColor(nodes.first()->getAttribute("color").value<QColor>());
    if (!color.isValid())
        return;

    for (auto node: nodes)
    {
        node->setAttribute("color", color);
    }

    addUndoState();
}


void CNodeEditorScene::onActionEdgeColor()
{
    QList<CEdge*> edges = getSelectedItems<CEdge>(true);
    if (edges.isEmpty())
        return;

    QColor color = QColorDialog::getColor(edges.first()->getAttribute("color").value<QColor>());
    if (!color.isValid())
        return;

    for (auto edge : edges)
    {
        edge->setAttribute("color", color);
    }

    addUndoState();
}


void CNodeEditorScene::onActionEdgeReverse()
{
    QList<CEdge*> edges = getSelectedItems<CEdge>(true);
    if (edges.isEmpty())
        return;

    for (auto edge : edges)
    {
        edge->reverse();
    }

    addUndoState();
}


void CNodeEditorScene::onActionEdgeDirected()
{
    QList<CEdge*> edges = getSelectedItems<CEdge>(true);
    if (edges.isEmpty())
        return;

    for (auto edge : edges)
    {
        edge->setAttribute("direction", "directed");
        edge->update();
    }

    addUndoState();
}


void CNodeEditorScene::onActionEdgeMutual()
{
    QList<CEdge*> edges = getSelectedItems<CEdge>(true);
    if (edges.isEmpty())
        return;

    for (auto edge : edges)
    {
        edge->setAttribute("direction", "mutual");
        edge->update();
    }

    addUndoState();
}


void CNodeEditorScene::onActionEdgeUndirected()
{
    QList<CEdge*> edges = getSelectedItems<CEdge>(true);
    if (edges.isEmpty())
        return;

    for (auto edge : edges)
    {
        edge->setAttribute("direction", "undirected");
        edge->update();
    }

    addUndoState();
}


bool CNodeEditorScene::populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems)
{
	if (!Super::populateMenu(menu, item, selectedItems))
		return false;

	// add default node actions
	menu.addSeparator();

	int nodesCount = getSelectedItems<CNode>(true).size();
	bool nodesSelected = (nodesCount > 0);

	QAction *linkAction = menu.addAction(tr("Link"), this, SLOT(onActionLink()));
	linkAction->setEnabled(nodesCount > 1);

	QAction *unlinkAction = menu.addAction(tr("Unlink"), this, SLOT(onActionUnlink()));
	unlinkAction->setEnabled(nodesSelected);

	QAction *nodeColorAction = menu.addAction(tr("Node(s) Color..."), this, SLOT(onActionNodeColor()));
	nodeColorAction->setEnabled(nodesSelected);

	// add default edge actions
	menu.addSeparator();

	bool edgesSelected = getSelectedItems<CEdge>(true).size();

	QAction *edgeColorAction = menu.addAction(tr("Connection(s) Color..."), this, SLOT(onActionEdgeColor()));
	edgeColorAction->setEnabled(edgesSelected);

	QMenu *arrowsMenu = menu.addMenu(tr("Direction"));
	arrowsMenu->setEnabled(edgesSelected);
	arrowsMenu->addAction(tr("Directed"), this, SLOT(onActionEdgeDirected()));
	arrowsMenu->addAction(tr("Mutual"), this, SLOT(onActionEdgeMutual()));
	arrowsMenu->addAction(tr("None"), this, SLOT(onActionEdgeUndirected()));
	arrowsMenu->addSeparator();
	arrowsMenu->addAction(tr("Reverse"), this, SLOT(onActionEdgeReverse()));

	return true;
}

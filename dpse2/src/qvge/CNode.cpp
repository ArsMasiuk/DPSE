#include "CNode.h"
#include "CEdge.h"
#include "CDirectEdge.h"

#include <QPen>
#include <QBrush>
#include <QEvent>
#include <QSet>
#include <QByteArray>
#include <QtMath>

// test
#include <QGraphicsDropShadowEffect>

////////////////////////////////////////////////////////////////////
/// \brief CNode::CNode

CNode::CNode(QGraphicsItem* parent) : QGraphicsRectItem(parent) 
{
	// no selection frame
	setItemFlag(IF_FramelessSelection);

	// default size
	resize(9);

	// default node flags
	m_nodeFlags = NF_OrphanAllowed;

	// default flags: movable & selectable
	auto flags = ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges | ItemUsesExtendedStyleOption;
	setFlags(flags);

	// accept hovers
	setAcceptHoverEvents(true);
	//setFiltersChildEvents(true);

	// cache
	setCacheMode(DeviceCoordinateCache);

	// label
	m_labelItem = new QGraphicsSimpleTextItem(this);
	m_labelItem->setFlags(0);
	m_labelItem->setCacheMode(DeviceCoordinateCache);
	m_labelItem->setPen(Qt::NoPen);
	m_labelItem->setAcceptedMouseButtons(Qt::NoButton);
	m_labelItem->setAcceptHoverEvents(false);


	// temp
	//addPort("Port 1", Qt::AlignLeft | Qt::AlignVCenter);
	//addPort("Port 2", Qt::AlignBottom | Qt::AlignRight);
	//addPort("Port 3", Qt::AlignBottom | Qt::AlignRight, 0, -20);
}


CNode::~CNode()
{
	for (CNodePort *port : m_ports)
	{
		port->onParentDeleted();
	}

	for (CEdge *conn : m_connections)
	{
		conn->onNodeDeleted(this);
	}
}


// reimp

QString CNode::createNewId() const
{
	static const QString tmpl("N%1");
	return createUniqueId<CNode>(tmpl);
}


void CNode::copyDataFrom(CItem* from)
{
	Super::copyDataFrom(from);

	CNode* fromNode = dynamic_cast<CNode*>(from);
	if (fromNode)
	{
		resize(fromNode->getSize());
		setPos(fromNode->pos());
		setZValue(fromNode->zValue());
	}

	updateCachedItems();
}


CItem* CNode::clone()
{
	CItem *item = create();
	CNode *node = dynamic_cast<CNode*>(item);

	if (scene())
		scene()->addItem(node);

	node->copyDataFrom(this);

	return item;
}


// attributes

bool CNode::hasLocalAttribute(const QByteArray& attrId) const
{
	if (/*attrId == "shape" || attrId == "size" ||*/ attrId == "pos" || 
		attrId == "x" || attrId == "y" || attrId == "z") 
		return true;

	return Super::hasLocalAttribute(attrId);
}


bool CNode::setAttribute(const QByteArray& attrId, const QVariant& v)
{
	setItemStateFlag(IS_Attribute_Changed);

	update();

	if (attrId == "shape")
	{
		Super::setAttribute(attrId, v);
		updateCachedItems();
		return true;
	}

	// virtual attributes
	if (attrId == "size")
	{
		if (v.type() == QVariant::Size || v.type() == QVariant::SizeF)
		{
			QSizeF sp = v.toSizeF();
			if (!sp.isNull())
			{
				Super::setAttribute(attrId, sp);
				resize(sp);
				updateCachedItems();
				return true;
			}
			return false;
		}

		float s = v.toFloat();
		if (s > 0)
		{
			Super::setAttribute(attrId, QSizeF(s,s));
			resize(s);
			updateCachedItems();
			return true;
		}

		return false;
	}

	if (attrId == "x")
	{
		setX(v.toDouble());
		return true;
	}

	if (attrId == "y")
	{
		setY(v.toDouble());
		return true;
	}

	if (attrId == "z")
	{
		setZValue(v.toDouble());
		return true;
	}

	if (attrId == "pos")
	{
		setPos(v.toPointF());
		return true;
	}

	return Super::setAttribute(attrId, v);
}


bool CNode::removeAttribute(const QByteArray& attrId)
{
	if (Super::removeAttribute(attrId))
	{
		updateCachedItems();
		return true;
	}
	else
		return false;
}


QVariant CNode::getAttribute(const QByteArray& attrId) const
{
	// virtual attributes
	if (attrId == "x")
	{
		return x();
	}

	if (attrId == "y")
	{
		return y();
	}

	if (attrId == "z")
	{
		return zValue();
	}

	if (attrId == "pos")
	{
		return pos();
	}

	return Super::getAttribute(attrId);
}


// ports

CNodePort* CNode::addPort(const QByteArray& portId, int align, double xoff, double yoff)
{
	if (portId.isEmpty() || m_ports.contains(portId))
		return NULL;

	CNodePort* port = new CNodePort(this, portId, align, xoff, yoff);
	m_ports[portId] = port;

	updateCachedItems();

	return port;
}


bool CNode::removePort(const QByteArray& portId)
{
	if (portId.isEmpty() || !m_ports.contains(portId))
		return false;

	CNodePort* port = m_ports.take(portId);

	// to do. update edges.

	delete port;

	updateCachedItems();

	return port;
}


CNodePort* CNode::getPort(const QByteArray& portId) const
{
	if (portId.isEmpty() || !m_ports.contains(portId))
		return NULL;
	else
		return m_ports[portId];
}


// serialization 

bool CNode::storeTo(QDataStream& out, quint64 version64) const
{
	out << getSize();

	out << pos() << itemFlags();

	out << zValue();

	// ports since v.11
	int portsCount = m_ports.size();
	out << portsCount;

	for (auto port: m_ports)
		port->storeTo(out, version64);

	return Super::storeTo(out, version64);
}


bool CNode::restoreFrom(QDataStream& out, quint64 version64)
{
	if (version64 >= 7)
	{
		QSizeF size; out >> size; resize(size);
	}
	else if (version64 > 0)
	{
		float size; out >> size; resize(size);
	}

	static QPointF p; out >> p; setPos(p);

	static int f; out >> f;
	//setItemFlags(f);

	if (version64 > 0)
	{
		qreal z; out >> z; setZValue(z);
	}

	// ports
	m_ports.clear();
	if (version64 >= 11)
	{
		int count = 0; 
		out >> count;
		
		QByteArray id;
		int align;
		double xoff, yoff;

		for (int i = 0; i < count; ++i)
		{
			out >> id;
			out >> align >> xoff >> yoff;

			CNodePort *port = new CNodePort(this, id, align, xoff, yoff);
			m_ports[id] = port;

			// update
			port->onParentGeometryChanged();
		}
	}

	return Super::restoreFrom(out, version64);
}


// node operations

bool CNode::merge(CNode *node, const QByteArray& portId)
{
	if (!node || (node == this))
		return false;

	bool allowCircled = allowCircledConnection();

	// make a copy because node's connections list will be updated
	QSet<CEdge*> toReconnect = node->m_connections;

	for (CEdge *conn : toReconnect)
	{
		conn->reattach(node, this, portId);
	}

	// kill old node
	delete node;	// will be removed by removed connections

	if (!allowCircled)
	{
		toReconnect = m_connections;

		for (CEdge *conn : toReconnect)
		{
			if (conn->isCircled())
				delete conn;
		}
	}

	return true;
}


QList<CNode*> CNode::unlink()
{
	QList<CNode*> nodes;

	float xpos = x();
	float ypos = y();

	float xstep = getScene() ? getScene()->getGridSize() : 10;

	while (m_connections.size() >= 2)
	{
		CEdge *c = *m_connections.begin();
		
		CNode *n = dynamic_cast<CNode*>(clone());
		xpos += xstep;
		n->setPos(xpos, ypos);

		c->reattach(this, n);

		nodes.append(n);
	}

	return nodes;
}


QList<CNode*> CNode::getCollidingNodes() const
{
	QList<CNode*> nodes;

	CEditorScene* scene = getScene();
	if (!scene)
		return nodes;

	QList<QGraphicsItem*> itemsUnderMouse = scene->items(pos());

	for (int i = 0; i < itemsUnderMouse.size(); ++i)
	{
		CNode *node = dynamic_cast<CNode*>(itemsUnderMouse.at(i));
		if (!node || node == this)
			continue;
		
		nodes.append(node);
	}

	return nodes;
}


QSet<CEdge*> CNode::getInConnections() const
{
    QSet<CEdge*> edges;

    for (auto edge: m_connections)
    {
        if (edge->lastNode() == this)
            edges << edge;
    }

    return edges;
}


QSet<CEdge*> CNode::getOutConnections() const
{
    QSet<CEdge*> edges;

    for (auto edge: m_connections)
    {
        if (edge->firstNode() == this)
            edges << edge;
    }

    return edges;
}


double CNode::getDistanceToLineEnd(const QLineF& line, const QByteArray& portId) const
{
	// port
	if (portId.size())
	{
		if (CNodePort* port = getPort(portId) )
		{
			double shift = (port->boundingRect().width() / 2);
			return shift;
		}
	}

	// circle 
	if (m_shapeCache.isEmpty())
	{
		double shift = qMax(rect().width() / 2, rect().height() / 2);
		return shift;
	}

	// polygon (must be cashed)
	QPolygonF scenePolygon = m_shapeCache.translated(pos());
    QPointF intersectionPoint = CUtils::closestIntersection(line, scenePolygon);
	return QLineF(intersectionPoint, line.p2()).length();
}


QPointF CNode::getIntersectionPoint(const QLineF& line) const
{
	// circle 
	if (m_shapeCache.isEmpty())
	{
		auto shift = qMax(rect().width() / 2, rect().height() / 2);
		auto angle = qDegreesToRadians(line.angle());
		return pos() + QPointF(shift * qCos(angle), shift * qSin(angle));
	}

	// polygon (must be cashed)
	QPolygonF scenePolygon = m_shapeCache.translated(pos());
	return CUtils::closestIntersection(line, scenePolygon);
}


///
/// \brief CNode::onConnectionAttach
/// \param conn
///
void CNode::onConnectionAttach(CEdge *conn)
{
	Q_ASSERT(conn != NULL);

	m_connections.insert(conn);

	updateConnections();
}

///
/// \brief CNode::onConnectionDetach
/// \param conn
///
void CNode::onConnectionDetach(CEdge *conn)
{
	Q_ASSERT(conn != NULL);

	m_connections.remove(conn);

	updateConnections();
}


void CNode::updateConnections()
{
	// optimize: no update while restoring
	if (s_duringRestore)
		return;

	typedef QList<CDirectEdge*> EdgeList;
	typedef QSet<QPair<CNode*, QByteArray>> Key;
	QHash<Key, EdgeList> edgeGroups;

	for (auto conn : m_connections)
	{
		CDirectEdge* dconn = dynamic_cast<CDirectEdge*>(conn);
		if (dconn)
		{
			QPair<CNode*, QByteArray> key1 = { dconn->firstNode(), dconn->firstPortId() };
			QPair<CNode*, QByteArray> key2 = { dconn->lastNode(), dconn->lastPortId() };
			Key key = { key1, key2 };
			edgeGroups[key].append(dconn);
		}
	}

	for (const EdgeList& values: edgeGroups)
	{
		if (values.count() == 1)
		{
			values.first()->setBendFactor(0);
		}
		else
		{
			if (values.first()->isCircled())
			{
				int bf = 0;

				for (auto conn : values)
				{
					conn->setBendFactor(bf++);
				}
			}
			else
			{
				int bf = (values.count() & 1) ? 0 : 1;

				for (auto conn : values)
				{
					conn->setBendFactor(bf);

					if (bf > 0)
						bf = 0 - bf;
					else
						bf = 1 - bf;
				}
			}
		}
	}
}


void CNode::onConnectionDeleted(CEdge *conn)
{
	onConnectionDetach(conn);

	// remove orphan node if allowed
	if (m_connections.isEmpty() && !(m_nodeFlags & NF_OrphanAllowed))
		delete this;
}


void CNode::onPortDeleted(CNodePort *port)
{
	for (auto edge : m_connections)
		edge->onNodePortDeleted(this, port->getId());

	m_ports.remove(port->getId());
}


void CNode::onItemMoved(const QPointF& /*delta*/)
{
	for (CEdge *conn : m_connections)
	{
		conn->onNodeMoved(this); 
	}
}


void CNode::onItemRestored()
{
	updateCachedItems();

	updateConnections();
}


void CNode::onDroppedOn(const QSet<IInteractive*>& acceptedItems, const QSet<IInteractive*>& /*rejectedItems*/)
{
	if (acceptedItems.size())
	{
		// check for ports
		for (auto item : acceptedItems)
		{
			CNodePort* port = dynamic_cast<CNodePort*>(item);
			if (port)
			{
				CNode* node = port->getNode();
				node->merge(this, port->getId());
				node->setSelected(true);
				return;
			}
		}

		// check for nodes
		for (auto item : acceptedItems)
		{
			CNode* node = dynamic_cast<CNode*>(item);
			if (node)
			{
				node->merge(this);
				node->setSelected(true);
				return;
			}
		}

		// nothing...
	}
}


ItemDragTestResult CNode::acceptDragFromItem(QGraphicsItem* draggedItem)
{
	if (dynamic_cast<CNode*>(draggedItem))
		return Accepted;

	return Ignored;
}


// reimp

QVariant CNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSceneHasChanged)
	{
		// update attributes cache after attach to scene
		updateCachedItems();

		// set default ID
		setDefaultId();

		return value;
	}

	if (change == ItemPositionHasChanged)
	{
		setItemStateFlag(IS_Attribute_Changed);

		QPointF d = value.toPointF() - scenePos();
		onItemMoved(d);

		return value;
	}

	if (change == ItemSelectedHasChanged)
	{
		onItemSelected(value.toBool());

		return value;
	}

	return value;
}


void CNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
	bool isSelected = (option->state & QStyle::State_Selected);

	painter->setClipRect(boundingRect());


	// get color (to optimize!)
	QColor color = getAttribute(QByteArrayLiteral("color")).value<QColor>();
	if (color.isValid())
		painter->setBrush(color);
	else
		painter->setBrush(getScene()->backgroundBrush());	// fake to fill with "transparent" brush


	QColor strokeColor = isSelected ? 
		QColor(QStringLiteral("orange")) : 
		getAttribute(QByteArrayLiteral("stroke.color")).value<QColor>();
	
	qreal strokeSize = getAttribute(QByteArrayLiteral("stroke.size")).toDouble();
	strokeSize = qMax(0.1, strokeSize);
	if (isSelected) strokeSize++;

	int strokeStyle = CUtils::textToPenStyle(getAttribute(QByteArrayLiteral("stroke.style")).toString(), Qt::SolidLine);

	painter->setPen(QPen(strokeColor, strokeSize, (Qt::PenStyle)strokeStyle));


	// hover opacity
	if (itemStateFlags() & IS_Drag_Accepted)
		painter->setOpacity(0.6);


	// draw shape: disc if no cache
	if (m_shapeCache.isEmpty())
	{
		QRectF r = Shape::boundingRect();
		painter->drawEllipse(r);
	}
	else
	{
		painter->drawPolygon(m_shapeCache);
	}
}


QRectF CNode::boundingRect() const
{
	QRectF r = Shape::boundingRect();

	// in case of bold selection
	return r.adjusted(-3, -3, 3, 3);
}


void CNode::updateCachedItems()
{
	auto shapeCache = m_shapeCache;
	auto sizeCache = m_sizeCache;

	recalculateShape();

	// update caches & connections 
	if (m_shapeCache != shapeCache || m_sizeCache != sizeCache)
	{
		prepareGeometryChange();

		// update ports
		updatePortsLayout();

		// update edges
		for (auto edge : m_connections)
		{
			edge->onParentGeometryChanged();
		}

		// update text label
		if (getScene() && getScene()->itemLabelsEnabled())
		{
			updateLabelPosition();
			updateLabelDecoration();
		}
	}
}


void CNode::updatePortsLayout()
{
	for (auto port : m_ports.values())
	{
		port->onParentGeometryChanged();
	}
}


void CNode::updateLabelPosition()
{
	int w = m_labelItem->boundingRect().width();
	int h = m_labelItem->boundingRect().height();

	QRectF r = Shape::boundingRect();
	if (r.width() < 30 || r.height() < 30)
		m_labelItem->setPos(-w / 2, boundingRect().height() / 2);	// if too small: put label on bottom
	else
		m_labelItem->setPos(-w / 2, -h / 2);		// else center
}


// priv

void CNode::recalculateShape()
{
	QSizeF sz = getAttribute("size").toSizeF();
	resize(sz);

	QRectF r = Shape::boundingRect();

	m_shapeCache.clear();
	m_sizeCache = r;

	QByteArray shapeType = getAttribute("shape").toByteArray();
	if (shapeType == "square")
	{
		m_shapeCache = r;
	}
	else if (shapeType == "diamond")
	{
		float rx = r.center().x();
		float ry = r.center().y();

		m_shapeCache << QPointF(rx, ry - r.height() / 2)
			<< QPointF(rx + r.width() / 2, ry)
			<< QPointF(rx, ry + r.height() / 2)
			<< QPointF(rx - r.width() / 2, ry)
			<< QPointF(rx, ry - r.height() / 2);
	}
	else if (shapeType == "hexagon")
	{
		float rx = r.center().x();
		float ry = r.center().y();

		m_shapeCache 
			<< QPointF(r.left() + r.width() / 3, ry - r.height() / 2)
			<< QPointF(r.left() + r.width() / 3 * 2, ry - r.height() / 2)
			<< QPointF(rx + r.width() / 2, ry)
			<< QPointF(r.left() + r.width() / 3 * 2, ry + r.height() / 2)
			<< QPointF(r.left() + r.width() / 3, ry + r.height() / 2)
			<< QPointF(r.left(), ry)
			<< QPointF(r.left() + r.width() / 3, ry - r.height() / 2);
	}
	else if (shapeType == "triangle")
	{
		m_shapeCache << r.bottomLeft() << r.bottomRight() << QPointF(r.topRight() + r.topLeft()) / 2 << r.bottomLeft();
	}
	else if (shapeType == "triangle2")
	{
		m_shapeCache << r.topLeft() << r.topRight() << QPointF(r.bottomRight() + r.bottomLeft()) / 2 << r.topLeft();
	}
	else // "disc"
	{
		// no cache
	}
}


// events

void CNode::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	m_internalStateFlags |= IS_Hover;

	onHoverEnter(this, event);

	update();
}


void CNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	m_internalStateFlags &= ~IS_Hover;

	onHoverLeave(this, event);

	update();
}


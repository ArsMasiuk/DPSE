#include "CNode.h"
#include "CConnection.h"

#include <QPen>
#include <QBrush>
#include <QEvent>

// test
#include <QGraphicsDropShadowEffect>

////////////////////////////////////////////////////////////////////
/// \brief CNode::CNode

CNode::CNode(QGraphicsItem* parent) : QGraphicsRectItem(parent) 
{
	// no selection frame
	setItemFlag(IF_FramelessSelection);

	// default node flags
	m_nodeFlags = 0;

	// default flags: movable & selectable
	auto flags = ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges;
	setFlags(flags);

	setRect(-5, -5, 11, 11);
	
	// accept hovers
	setAcceptHoverEvents(true);

	// cache
	setCacheMode(DeviceCoordinateCache);

	// label
	m_labelItem = new QGraphicsTextItem(this);
	m_labelItem->setCacheMode(DeviceCoordinateCache);

	// test
	//auto effect = new QGraphicsDropShadowEffect();
	//effect->setBlurRadius(10);
	//setGraphicsEffect(effect);
}


CNode::~CNode()
{
	for (CConnection *conn : m_connections)
	{
		conn->onNodeDeleted(this);
	}
}


// reimp

QString CNode::createNewId() const
{
	static int count = 0;

	return QString("N%1").arg(++count);
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

bool CNode::setAttribute(const QByteArray& attrId, const QVariant& v)
{
	setItemStateFlag(IS_Attribute_Changed);

	update();

	if (attrId == "size")
	{
		float s = v.toFloat();
		if (s > 0)
		{
			prepareGeometryChange();
			resize(s);
		}
		return true;
	}

	// virtual attributes
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


QVariant CNode::getAttribute(const QByteArray& attrId) const
{
	if (attrId == "size")
	{
		return getSize();
	}

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


// serialization 

bool CNode::storeTo(QDataStream& out, quint64 version64) const
{
	if (version64 > 0)
	{
		out << getSize();
	}

	out << pos() << itemFlags();

	if (version64 > 0)
	{
		qreal zv = zValue();
		out << zv;
	}

	return Super::storeTo(out, version64);
}


bool CNode::restoreFrom(QDataStream& out, quint64 version64)
{
	if (version64 > 0)
	{
		float size; out >> size; resize(size);
	}

	static QPointF p; out >> p; setPos(p);

	int f; out >> f;
	//setItemFlags(f);

	if (version64 > 0)
	{
		qreal z; out >> z; setZValue(z);
	}

	return Super::restoreFrom(out, version64);
}


// node operations

void CNode::merge(CNode *node)
{
	if (!node || (node == this))
		return;

	bool allowCircled = allowCircledConnection();

	// make a copy because node's connections list will be updated
	QSet<CConnection*> toReconnect = node->m_connections;

	for (CConnection *conn : toReconnect)
	{
		conn->reattach(node, this);
	}

	// kill old node
	delete node;	// will be removed by removed connections

	if (!allowCircled)
	{
		toReconnect = m_connections;

		for (CConnection *conn : toReconnect)
		{
			if (conn->isCircled())
				delete conn;
		}
	}
}


QList<CNode*> CNode::unlink()
{
	QList<CNode*> nodes;

	float xpos = x();
	float ypos = y();

	float xstep = getScene() ? getScene()->getGridSize() : 10;

	while (m_connections.size() >= 2)
	{
		CConnection *c = *m_connections.begin();
		
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


double CNode::getDistanceToLineEnd(const QLineF& line) const
{
	// circle 
	if (m_shapeCache.isEmpty())
	{
		double shift = qMax(rect().width() / 2, rect().height() / 2);
		return shift;
	}

	// polygon (must be cashed)
	QPolygonF scenePolygon = m_shapeCache.translated(pos());
	QPointF intersectionPoint = Utils::closestIntersection(line, scenePolygon);
	return QLineF(intersectionPoint, line.p2()).length();
}


///
/// \brief CNode::onConnectionAttach
/// \param conn
///
void CNode::onConnectionAttach(CConnection *conn)
{
	Q_ASSERT(conn != NULL);

	m_connections.insert(conn);

	updateConnections();
}

///
/// \brief CNode::onConnectionDetach
/// \param conn
///
void CNode::onConnectionDetach(CConnection *conn)
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

	typedef QList<CConnection*> EdgeList;
	QMap<CNode*, EdgeList> edgeGroups;

	for (auto conn : m_connections)
	{
		CNode* node = conn->firstNode() == this ? conn->lastNode() : conn->firstNode();
		edgeGroups[node].append(conn);
	}

	for (auto it = edgeGroups.constBegin(); it != edgeGroups.constEnd(); ++it)
	{
		const EdgeList& values = it.value();

		if (values.count() == 1)
		{
			values.first()->setBendFactor(0);
		}
		else
		{
			if (values.first()->isCircled())
			{
				int bf = 1;

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


void CNode::onConnectionDeleted(CConnection *conn)
{
	onConnectionDetach(conn);

	// remove orphan node if allowed
	if (m_connections.isEmpty() && !(m_nodeFlags & NF_OrphanAllowed))
		delete this;
}


void CNode::onItemMoved()
{
	for (CConnection *conn : m_connections)
	{
		conn->onNodeMoved(this);
	}
}


void CNode::onItemRestored()
{
	updateConnections();
}


void CNode::onDroppedOn(const QSet<CItem*>& acceptedItems, const QSet<CItem*>& /*rejectedItems*/)
{
	if (acceptedItems.size())
	{
		CNode* node = dynamic_cast<CNode*>(*acceptedItems.begin());
		if (node)
			node->merge(this);
	}
}


// reimp

QVariant CNode::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsItem::ItemSceneHasChanged)
	{
		// set default ID
		setDefaultId();

		return value;
	}

	if (change == QGraphicsItem::ItemPositionHasChanged)
	{
		setItemStateFlag(IS_Attribute_Changed);

		onItemMoved();

		return value;
	}

	if (change == QGraphicsItem::ItemPositionChange && getScene())
	{
		if (getScene()->gridSnapEnabled())
		{
			int snapStep = getScene()->getGridSize();

			QPointF newPos = value.toPointF();

			if (newPos.x() < 0)
				newPos.setX(newPos.x() - snapStep / 2);
			else
				newPos.setX(newPos.x() + snapStep / 2);

			if (newPos.y() < 0)
				newPos.setY(newPos.y() - snapStep / 2);
			else
				newPos.setY(newPos.y() + snapStep / 2);

			newPos.setX((int)newPos.x() - (int)newPos.x() % snapStep);
			newPos.setY((int)newPos.y() - (int)newPos.y() % snapStep);

			return newPos;
		}
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
	painter->setClipRect(option->exposedRect);

	QRectF r = Shape::boundingRect();

	// get color (to optimize!)
	QVariant color = getAttribute("color");
	if (color.canConvert<QColor>())
		painter->setBrush(color.value<QColor>());
	else
		painter->setBrush(Qt::cyan);

	bool isSelected = (option->state & QStyle::State_Selected);
	if (isSelected)
		painter->setPen(QPen(QColor("orange"), 2));
	else
		painter->setPen(QPen(Qt::black, 1));

	// get shape (to optimize!)
	QPolygonF shapeCache;

	QByteArray shapeType = getAttribute("shape").toByteArray();
	if (shapeType == "square")
	{
		shapeCache = r;
		painter->drawRect(r);
	}
	else if (shapeType == "diamond")
	{
		float rx = r.center().x();
		float ry = r.center().y();

		shapeCache << QPointF(rx, ry - r.height() / 2)
			<< QPointF(rx + r.width() / 2, ry)
			<< QPointF(rx, ry + r.height() / 2)
			<< QPointF(rx - r.width() / 2, ry)
			<< QPointF(rx, ry - r.height() / 2);
		painter->drawPolygon(shapeCache);
	}
	else if (shapeType == "triangle")
	{
		shapeCache << r.bottomLeft() << r.bottomRight() << QPointF(r.topRight() + r.topLeft()) / 2 << r.bottomLeft();
		painter->drawPolygon(shapeCache);
	}
	else // "disc"
	{
		shapeCache.clear();
		painter->drawEllipse(r);
	}

	// draw text label
	if (getScene()->itemLabelsEnabled())
	{
		updateLabelPosition();
		updateLabelDecoration();
	}

	// update caches & connections 
	if (m_shapeCache != shapeCache || m_sizeCache != rect())
	{
		m_shapeCache = shapeCache;
		m_sizeCache = rect();

		for (auto edge : m_connections)
			edge->update();	
	}
}


QRectF CNode::boundingRect() const
{
	QRectF r = Shape::boundingRect();

	// in case of bold selection
	return r.adjusted(-3, -3, 3, 3);
}


void CNode::updateLabelPosition()
{
	int w = m_labelItem->boundingRect().width();

    m_labelItem->setPos(-w / 2, boundingRect().height() / 2);
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


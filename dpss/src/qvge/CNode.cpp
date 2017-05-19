#include "CNode.h"
#include "CConnection.h"

#include <QPen>
#include <QBrush>

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

	if (attrId == "size")
	{
		float s = v.toFloat();
		if (s > 0)
			resize(s);
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

void CNode::merge(CNode *node, bool allowCircled)
{
	if (node == this || !node)
		return;

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


bool CNode::allowStartConnection() const
{
	return true;
}


///
/// \brief CNode::onConnectionAttach
/// \param conn
///
void CNode::onConnectionAttach(CConnection *conn)
{
	Q_ASSERT(conn != NULL);
	//Q_ASSERT(!m_connections.contains(conn));

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
	//QMap<CNode*, int> indexMap;

	//for (auto conn : m_connections)
	//{
	//	if (!conn->isCircled())
	//	{
	//		CNode* node = conn->firstNode() == this ? conn->lastNode() : conn->firstNode();
	//		
	//		if (indexMap.contains(node))
	//		{
	//			int mi = indexMap[node];
	//			indexMap[node] = ++mi;
	//			
	//			int bf = (mi + 1) / 2;
	//			if (mi & 1)
	//				bf = -bf;

	//			conn->setBendFactor(bf);
	//		}
	//		else
	//		{
	//			indexMap[node] = 0;
	//			
	//			conn->setBendFactor(0);
	//		}
	//	}
	//}

	typedef QSet<CNode*> MultiIndex;

	QMultiHash<MultiIndex, CConnection*> multiHash;

	for (auto conn : m_connections)
	{
		MultiIndex index;
		index.insert(conn->firstNode());
		index.insert(conn->lastNode());

		multiHash.insert(index, conn);
	}

	for (auto it = multiHash.constBegin(); it != multiHash.constEnd(); it++)
	{
		auto values = multiHash.values(it.key());

		if (values.count() == 1)
		{
			values.first()->setBendFactor(0);
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

	return value;
}


void CNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * /*widget*/)
{
	updateTextInfo();

	QRectF r = Shape::boundingRect();

	// get color (to optimize!)
	QVariant color = getAttribute("color");
	if (color.canConvert<QColor>())
		painter->setBrush(color.value<QColor>());
	else
		painter->setBrush(Qt::cyan);

	bool isSelected = (option->state & QStyle::State_Selected);
	if (isSelected)
	{
		painter->setPen(QPen(QColor("orange"), 2));
	}
	else
	{
		painter->setPen(QPen(Qt::black, 1));
	}

	// get shape (to optimize!)
	QByteArray shapeType = getAttribute("shape").toByteArray();
	if (shapeType == "square")
	{
		painter->drawRect(r);
	}
	else if (shapeType == "diamond")
	{
		float rx = r.center().x();
		float ry = r.center().y();

		QPolygonF pf;
		pf << QPointF(rx, ry - r.height() / 2)
			<< QPointF(rx + r.width() / 2, ry)
			<< QPointF(rx, ry + r.height() / 2)
			<< QPointF(rx - r.width() / 2, ry)
			<< QPointF(rx, ry - r.height() / 2);
		painter->drawPolygon(pf);
	}
	else if (shapeType == "triangle")
	{
		QPolygonF pf;
		pf << r.bottomLeft() << r.bottomRight() << QPointF(r.topRight() + r.topLeft()) / 2 << r.bottomLeft();
		painter->drawPolygon(pf);
	}
	else // "disc"
	{
		painter->drawEllipse(r);
	}

	// draw text label
	if (getScene()->itemLabelsEnabled())
		drawLabel(painter, option);
}


QRectF CNode::boundingRect() const
{
	QRectF r = Shape::boundingRect();

	if (getScene()->itemLabelsEnabled() && m_label && !m_label->text().isEmpty())
	{
		QPointF p = labelOffset(r, m_label->localSize());
		r |= QRectF(p, m_label->localSize());
	}

	// in case of bold selection
	return r.adjusted(-3, -3, 3, 3);
}


void CNode::hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/)
{
	m_internalStateFlags |= IS_Hover;

	onHoverEnter();

	update();
}


void CNode::hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/)
{
	m_internalStateFlags &= ~IS_Hover;

	onHoverLeave();

	update();
}

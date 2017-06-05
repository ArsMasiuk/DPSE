/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CConnection.h"
#include "CNode.h"

#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#define _USE_MATH_DEFINES
#include <math.h>


CConnection::CConnection(QGraphicsItem *parent): Shape(parent)
{
    m_firstNode = m_lastNode = NULL;

	m_bendFactor = 2;

    setZValue(-1);

	// non-movable but selectable
	auto flags = ItemIsSelectable | ItemSendsGeometryChanges;
	setFlags(flags);
	
	// no selection frame
	setItemFlag(IF_FramelessSelection);

	// accept hovers
	setAcceptHoverEvents(true);

	// cache
	setCacheMode(DeviceCoordinateCache);

	// test
	m_labelItem = new QGraphicsTextItem(this);
	m_labelItem->setCacheMode(DeviceCoordinateCache);
}


CConnection::~CConnection()
{
	if (m_firstNode)
		m_firstNode->onConnectionDeleted(this);

	if (m_lastNode && m_lastNode != m_firstNode)
		m_lastNode->onConnectionDeleted(this);
}


// attributes

bool CConnection::setAttribute(const QByteArray& attrId, const QVariant& v)
{
	if (attrId == "direction")
	{
		updateArrowFlags(v.toString());
	}

	bool res = Super::setAttribute(attrId, v);

	if (res) update();
	return res;
}


bool CConnection::removeAttribute(const QByteArray& attrId)
{
	bool res = Super::removeAttribute(attrId);

	if (attrId == "direction")
	{
		updateArrowFlags(getAttribute("direction").toString());
	}

	if (res) update();
	return res;
}


// cached attributes

void CConnection::updateCachedItems()
{
	Super::updateCachedItems();

	updateArrowFlags(getAttribute("direction").toString());
}


void CConnection::updateArrowFlags(const QString& direction)
{
	if (direction == "directed")
	{
		setItemFlag(CF_End_Arrow);
		resetItemFlag(CF_Start_Arrow);
	}
	else if (direction == "mutual")
	{
		setItemFlag(CF_Mutual_Arrows);
	}
	else if (direction == "undirected")
	{
		resetItemFlag(CF_Mutual_Arrows);
	}
}


// reimp

QPainterPath CConnection::shape() const
{
	return m_shapePath;
}


void CConnection::setupPainter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* /*widget*/)
{
	// weight
	bool ok = false;
	double weight = qMax(0.1, getAttribute("weight").toDouble(&ok));
	if (!ok) weight = 1;
	if (weight > 10) weight = 10;	// safety

	// line style
	Qt::PenStyle penStyle = Qt::SolidLine;
	QString lineStyle = getAttribute("style").toString();
	if (lineStyle == "dotted")
		penStyle = Qt::DotLine;
	else if (lineStyle == "dashed")
		penStyle = Qt::DashLine;

	// color & selection
	bool isSelected = (option->state & QStyle::State_Selected);
	if (isSelected)
	{
		painter->setPen(QPen(QColor("orange"), weight + 1, penStyle));
	}
	else
	{
		// get color (to optimize!)
		QColor color = getAttribute("color").value<QColor>();

		painter->setPen(QPen(color, weight, penStyle));
	}
}


void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
	// called before draw 
	setupPainter(painter, option, widget);

	// straight line
	if (m_bendFactor == 0)
	{
		painter->drawLine(line());

		// arrows
		if (m_itemFlags & CF_Start_Arrow)
			drawArrow(painter, option, true, QLineF(line().p2(), line().p1()));

		if (m_itemFlags & CF_End_Arrow)
			drawArrow(painter, option, false, line());
	}
	else
	{
		QPainterPath pp;
		pp.moveTo(line().p1());
		pp.cubicTo(m_controlPoint, m_controlPoint, line().p2());

		painter->setBrush(Qt::NoBrush);
		painter->drawPath(pp);

		// arrows
		if (m_itemFlags & CF_Start_Arrow)
			drawArrow(painter, option, true, QLineF(m_controlPos, line().p1()));

		if (m_itemFlags & CF_End_Arrow)
			drawArrow(painter, option, false, QLineF(m_controlPos, line().p2()));
	}

	// draw text label
	if (getScene()->itemLabelsEnabled())
		updateLabelPosition();
}


void CConnection::drawArrow(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, bool first, const QLineF& direction)
{
	qreal shift = 0;
	if (first && m_firstNode)
	{
		shift = m_firstNode->getDistanceToLineEnd(direction);
	}
	else if (!first && m_lastNode)
	{
		shift = m_lastNode->getDistanceToLineEnd(direction);
	}

	static QPolygonF arrowHead;
	if (arrowHead.isEmpty())
		arrowHead << QPointF(0, 0) << QPointF(-3, 6) << QPointF(3, 6) << QPointF(0, 0);

	painter->save();

	painter->setBrush(painter->pen().color());

	static QLineF hl(0, 0, 0, 100);
	qreal a = direction.angleTo(hl);

	painter->translate(direction.p2());
	painter->rotate(180 + a);
	painter->translate(QPointF(0, shift + 1));
	painter->drawPolygon(arrowHead);

	painter->restore();
}


void CConnection::updateLabelPosition()
{
	int w = m_labelItem->boundingRect().width();

	m_labelItem->setPos(m_controlPos.x() - w / 2, m_controlPos.y());
}


// IO 

bool CConnection::storeTo(QDataStream &out, quint64 version64) const
{
	Super::storeTo(out, version64);

    out << quint64(m_firstNode) << quint64(m_lastNode);

	return true;
}


bool CConnection::restoreFrom(QDataStream &out, quint64 version64)
{
	if (Super::restoreFrom(out, version64))
	{
		// these are TEMP ids
        quint64 t1, t2; out >> t1 >> t2;
		m_firstNode = (CNode*)t1;
		m_lastNode = (CNode*)t2;

		return true;
	}

	return false;
}


bool CConnection::linkAfterRestore(const CItemLinkMap &idToItem)
{
    CNode *node1 = dynamic_cast<CNode*>(idToItem.value((quint64)m_firstNode));
    CNode *node2 = dynamic_cast<CNode*>(idToItem.value((quint64)m_lastNode));

	m_firstNode = m_lastNode = NULL;

	setFirstNode(node1);
	setLastNode(node2);

	return true;
}


bool CConnection::linkAfterPaste(const CItemLinkMap& idToItem)
{
	bool res = linkAfterRestore(idToItem);

	return res && isValid();
}


// impl

void CConnection::setFirstNode(CNode *node)
{
    if (m_firstNode)
        m_firstNode->onConnectionDetach(this);

    m_firstNode = node;

    if (m_firstNode)
        m_firstNode->onConnectionAttach(this);

	onPositionUpdated();
}


void CConnection::setLastNode(CNode *node)
{
    if (m_lastNode)
        m_lastNode->onConnectionDetach(this);

    m_lastNode = node;

    if (m_lastNode)
        m_lastNode->onConnectionAttach(this);

	onPositionUpdated();
}


void CConnection::reattach(CNode *oldNode, CNode *newNode)
{
	if (oldNode == newNode)
		return;

	if (m_firstNode == oldNode)
		setFirstNode(newNode);

	if (m_lastNode == oldNode)
		setLastNode(newNode);
}


void CConnection::reverse()
{
	qSwap(m_firstNode, m_lastNode);

	onPositionUpdated();
}


void CConnection::setBendFactor(int bf) 
{ 
	m_bendFactor = bf; 

	onPositionUpdated();
}


// reimp

CConnection* CConnection::clone()
{
	CConnection* c = new CConnection();
	c->setFirstNode(m_firstNode);
	c->setLastNode(m_lastNode);
	return c;
}


QString CConnection::createNewId() const
{
	static int count = 0;

	return QString("C%1").arg(++count);
}


// callbacks

void CConnection::onNodeMoved(CNode *node)
{
	Q_ASSERT(node == m_firstNode || node == m_lastNode);
	Q_ASSERT(node != NULL);

	onPositionUpdated();
}


void CConnection::onNodeDetached(CNode *node)
{
	if (node == m_firstNode)
	{
		m_firstNode = NULL;
	}

	if (node == m_lastNode)
	{
		m_lastNode = NULL;
	}
}


void CConnection::onNodeDeleted(CNode *node)
{
	onNodeDetached(node);

	delete this;	// die as well
}


// protected

void CConnection::onPositionUpdated()
{
	if (!m_firstNode || !m_lastNode)
		return;

	// update line position
	QPointF p1 = m_firstNode->pos(), p2 = m_lastNode->pos();
	setLine(QLineF(p1, p2));

	// update shape path
	QPainterPath path;
	path.moveTo(p1);

	// center
	m_controlPos = (p1 + p2) / 2;
	
	if (m_bendFactor == 0)
	{
		path.lineTo(p2);
	}
	else
	{
		QPointF t1 = m_controlPos;
		float posFactor = qAbs(m_bendFactor);

		bool bendDirection = (quint64(m_firstNode) > quint64(m_lastNode));
		if (m_bendFactor < 0)
			bendDirection = !bendDirection;

		QLineF f1(t1, p2);
		f1.setAngle(bendDirection ? f1.angle() + 90 : f1.angle() - 90);
		f1.setLength(f1.length() * 0.2 * posFactor);

		m_controlPos = f1.p2();
		m_controlPoint = m_controlPos - (t1 - m_controlPos) * 0.33;

		path.cubicTo(m_controlPoint, m_controlPoint, p2);
	}
	
	QPainterPathStroker stroker;
	stroker.setWidth(6);
	m_shapePath = stroker.createStroke(path);
}


// reimp

QVariant CConnection::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemSceneHasChanged)
	{
		// set default ID
		setDefaultId();

		return value;
	}

	if (change == ItemPositionChange)
	{
		// discard any movement
		return QVariant();
	}

	return value;
}


void CConnection::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	onHoverEnter(this, event);
}


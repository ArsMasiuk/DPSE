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
#include <QDebug>

#define _USE_MATH_DEFINES
#include <math.h>


const int ARROW_SIZE = 6;


CConnection::CConnection(QGraphicsItem *parent): Shape(parent)
{
    m_firstNode = m_lastNode = NULL;

	m_bendFactor = 0;

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

	// label
	m_labelItem = new QGraphicsSimpleTextItem(this);
	m_labelItem->setFlags(0);
	m_labelItem->setCacheMode(DeviceCoordinateCache);
	m_labelItem->setPen(Qt::NoPen);
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
	return m_selectionShapePath;
}


QRectF CConnection::boundingRect() const
{
	//QRectF r(line().p1(), line().p2());
	//r = r.united(QRectF(m_controlPos, m_controlPos + QPointF(1,1)));
	//return r.adjusted(-2,-2,2,2);
	return Shape::boundingRect();
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
		painter->setPen(QPen(QColor("orange"), weight + 1.0, penStyle, Qt::FlatCap, Qt::MiterJoin));
	}
	else
	{
		// get color (to optimize!)
		QColor color = getAttribute("color").value<QColor>();

		painter->setPen(QPen(color, weight, penStyle, Qt::FlatCap, Qt::MiterJoin));
	}
}


void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
	//painter->setClipRect(option->exposedRect);

	// called before draw 
	setupPainter(painter, option, widget);

	// circled connection
	if (isCircled())
	{
		int nodeDiameter = m_firstNode->boundingRect().height();
		double nr = nodeDiameter / 2;
		double r = nr + qAbs(m_bendFactor) * nr / 2;

		painter->drawEllipse(m_controlPos, r, r);
	}
	else 
	if (m_bendFactor == 0)	// straight line
	{
		painter->drawLine(line());

		// arrows
		if (m_itemFlags & CF_Start_Arrow)
			drawArrow(painter, option, true, QLineF(line().p2(), line().p1()));

		if (m_itemFlags & CF_End_Arrow)
			drawArrow(painter, option, false, line());
	}
	else // curve
	{
		QPainterPath pp;
		pp.moveTo(line().p1());
		pp.cubicTo(m_controlPoint, m_controlPoint, line().p2());

		painter->setBrush(Qt::NoBrush);
		painter->drawPath(pp);

		// arrows
		if (m_itemFlags & CF_Start_Arrow)
		{
			QLineF arrowLine = calculateArrowLine(pp, true, QLineF(m_controlPos, line().p1()));
			drawArrow(painter, option, true, arrowLine);
		}

		if (m_itemFlags & CF_End_Arrow)
		{
			QLineF arrowLine = calculateArrowLine(pp, false, QLineF(m_controlPos, line().p2()));
			drawArrow(painter, option, false, arrowLine);
		}
	}

}


QLineF CConnection::calculateArrowLine(const QPainterPath &path, bool first, const QLineF &direction) const
{
	qreal len = path.length();

	if (first && m_firstNode)
	{
		qreal shift = m_firstNode->getDistanceToLineEnd(direction);
		qreal arrowStart = path.percentAtLength(shift + ARROW_SIZE);
		return QLineF(path.pointAtPercent(arrowStart), direction.p2());
	}
	else if (!first && m_lastNode)
	{
		qreal shift = m_lastNode->getDistanceToLineEnd(direction);
		qreal arrowStart = path.percentAtLength(len - shift - ARROW_SIZE);
		return QLineF(path.pointAtPercent(arrowStart), direction.p2());
	}

	return direction;
}


void CConnection::drawArrow(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, bool first, const QLineF& direction) const
{
	if (first && m_firstNode)
	{
		qreal shift = m_firstNode->getDistanceToLineEnd(direction);
		drawArrow(painter, shift, direction);
	}
	else if (!first && m_lastNode)
	{
		qreal shift = m_lastNode->getDistanceToLineEnd(direction);
		drawArrow(painter, shift, direction);
	}
}


void CConnection::drawArrow(QPainter* painter, qreal shift, const QLineF& direction) const
{
	static QPolygonF arrowHead;
	if (arrowHead.isEmpty())
		arrowHead << QPointF(0, 0) << QPointF(-ARROW_SIZE/2, ARROW_SIZE) << QPointF(ARROW_SIZE/2, ARROW_SIZE) << QPointF(0, 0);

	painter->save();

	painter->setBrush(painter->pen().color());

	static QLineF hl(0, 0, 0, 100);
	qreal a = direction.angleTo(hl);

	painter->translate(direction.p2());
	painter->rotate(180 + a);
	painter->translate(QPointF(0, shift + painter->pen().widthF()));
	painter->drawPolygon(arrowHead);

	painter->restore();
}


void CConnection::updateLabelPosition()
{
	auto r = m_labelItem->boundingRect();
	int w = r.width();
	int h = r.height();
	m_labelItem->setTransformOriginPoint(w / 2, h / 2);

	if (isCircled())
	{
		m_labelItem->setPos(m_controlPos.x() - w / 2, m_controlPos.y() - boundingRect().height() / 2 - h);

		m_labelItem->setRotation(0);
	}
	else
	{
		m_labelItem->setPos(m_controlPos.x() - w / 2, m_controlPos.y() - h / 2);

		// update label rotation
		qreal angle = 180 - line().angle();
		if (angle > 90) angle -= 180;
		else if (angle < -90) angle += 180;
		//qDebug() << angle;
		m_labelItem->setRotation(angle);
	}
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
        out >> m_tempFirstNodeId >> m_tempLastNodeId;

		return true;
	}

	return false;
}


bool CConnection::linkAfterRestore(const CItemLinkMap &idToItem)
{
    CNode *node1 = dynamic_cast<CNode*>(idToItem.value(m_tempFirstNodeId));
    CNode *node2 = dynamic_cast<CNode*>(idToItem.value(m_tempLastNodeId));

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

	onParentGeometryChanged();
}


void CConnection::setLastNode(CNode *node)
{
    if (m_lastNode)
        m_lastNode->onConnectionDetach(this);

    m_lastNode = node;

    if (m_lastNode)
        m_lastNode->onConnectionAttach(this);

	onParentGeometryChanged();
}


void CConnection::reattach(CNode *oldNode, CNode *newNode)
{
	if (oldNode == newNode && !newNode->allowCircledConnection())
		return;

	if (m_firstNode == oldNode)
		setFirstNode(newNode);

	if (m_lastNode == oldNode)
		setLastNode(newNode);
}


void CConnection::reverse()
{
	qSwap(m_firstNode, m_lastNode);

	onParentGeometryChanged();
}


void CConnection::setBendFactor(int bf) 
{ 
	if (bf != m_bendFactor)
	{
		m_bendFactor = bf;

		onParentGeometryChanged();
	}
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

	onParentGeometryChanged();
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

void CConnection::onParentGeometryChanged()
{
	if (!m_firstNode || !m_lastNode)
		return;

	// optimize: no update while restoring
	if (s_duringRestore)
		return;

	prepareGeometryChange();

	// update line position
	QPointF p1 = m_firstNode->pos(), p2 = m_lastNode->pos();
	QLineF l(p1, p2);
	setLine(l);

	// update shape path
	QPainterPath path;
	
	// circled connection 
	if (isCircled())
	{
		int nodeDiameter = m_firstNode->boundingRect().height();
		double nr = nodeDiameter / 2;
		double r = nr + qAbs(m_bendFactor) * nr / 2;
		
		m_controlPos = p1 + QPointF(0, -r);
		path.addEllipse(m_controlPos, r, r);
	}
	else // not circled
	{
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
	}
	
	QPainterPathStroker stroker;
	stroker.setWidth(6);
	m_selectionShapePath = stroker.createStroke(path);

	update();

	// update text label
	if (getScene() && getScene()->itemLabelsEnabled())
	{
		updateLabelPosition();
		updateLabelDecoration();
	}
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

	if (change == ItemSelectedHasChanged)
	{
		onItemSelected(value.toBool());

		return value;
	}

	return value;
}


void CConnection::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	onHoverEnter(this, event);
}


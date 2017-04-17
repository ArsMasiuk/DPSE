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
}

CConnection::~CConnection()
{
	if (m_firstNode)
		m_firstNode->onConnectionDeleted(this);

	if (m_lastNode && m_lastNode != m_firstNode)
		m_lastNode->onConnectionDeleted(this);
}

// reimp

QPainterPath CConnection::shape() const
{
	return m_shapePath;
}

void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* /*widget*/)
{
	updateTextInfo();

	// weight
	bool ok = false;
	double weight = qMax(0.0, getAttribute("weight").toDouble(&ok));
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
		painter->setPen(QPen(QColor("orange"), weight+1, penStyle));
	}
	else
	{
		// get color (to optimize!)
		QColor color = getAttribute("color").value<QColor>();

		painter->setPen(QPen(color, weight, penStyle));
	}

	// straight line
	if (m_bendFactor == 0)
	{
		painter->drawLine(line());

		// arrows
		if (m_itemFlags & CF_Start_Arrow)
			drawArrow(painter, option, true, line());

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
			drawArrow(painter, option, true, QLineF(line().p1(), m_controlPos));

		if (m_itemFlags & CF_End_Arrow)
			drawArrow(painter, option, false, QLineF(m_controlPos, line().p2()));
	}

	// draw text label
	if (getScene()->itemLabelsEnabled())
		drawLabel(painter, option);
}

QPointF CConnection::labelOffset(const QRectF& /*itemRect*/, const QSizeF& labelSize) const
{
	// center label over line rect
	return m_controlPos - QPointF(labelSize.width() / 2, -labelSize.height() / 2);
}

void CConnection::drawArrow(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, bool first, const QLineF& direction)
{
	qreal shift = 0;
	if (first && m_firstNode)
	{
		QRectF rect = m_firstNode->rect();
		shift = qMax(rect.width() / 2, rect.height() / 2);
	}
	else if (!first && m_lastNode)
	{
		QRectF rect = m_lastNode->rect();
		shift = qMax(rect.width() / 2, rect.height() / 2);
	}

	//qreal lineLength = line().length() - shift;

	//qreal arrowSize = qMin((qreal)10, (qreal)lineLength / 3);
	//if (arrowSize <= 1)
	//	return;

	static QPolygonF arrowHead;
	if (arrowHead.isEmpty())
		arrowHead << QPointF(0, 0) << QPointF(-3, 6) << QPointF(3, 6) << QPointF(0, 0);

	painter->save();

	// painter->setPen(painter->pen().color().darker());
	painter->setBrush(painter->pen().color());

	static QLineF hl(0, 0, 0, 100);
	qreal a = direction.angleTo(hl);

	painter->translate(first ? direction.p1() : direction.p2());
	painter->rotate(first ? a : 180 + a);
	painter->translate(QPointF(0, shift+1));
	painter->drawPolygon(arrowHead);

	painter->restore();
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

	setVisible(true);

	return true;
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
	if (change == QGraphicsItem::ItemSceneHasChanged)
	{
		// set default ID
		setDefaultId();

		return value;
	}

	return value;
}


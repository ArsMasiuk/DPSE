/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include <QDebug>

#include "CDirectEdge.h"
#include "CNode.h"


CDirectEdge::CDirectEdge(QGraphicsItem *parent): Super(parent)
{
	m_bendFactor = 0;
}


void CDirectEdge::setBendFactor(int bf)
{
	if (bf != m_bendFactor)
	{
		m_bendFactor = bf;

		onParentGeometryChanged();
	}
}


// reimp

CEdge* CDirectEdge::clone()
{
	CDirectEdge* c = new CDirectEdge(parentItem());

	//c->setFirstNode(m_firstNode);
	//c->setLastNode(m_lastNode);

	// assign directly!
	c->m_firstNode = m_firstNode;
	c->m_firstPortId = m_firstPortId;
	c->m_lastNode = m_lastNode;
	c->m_lastPortId = m_lastPortId;

	if (scene())
		scene()->addItem(c);

	c->copyDataFrom(this);

	return c;
}


void CDirectEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget* widget)
{
	//qDebug() << boundingRect() << option->exposedRect << option->rect;

	// called before draw 
    setupPainter(painter, option, widget);

    painter->setClipRect(boundingRect());

	auto len = line().length();
	bool isArrow = (len > ARROW_SIZE * 2);

	bool isDirect = (!isCircled() && (m_bendFactor == 0));
	if (isDirect)	// straight line
	{
		painter->setBrush(Qt::NoBrush);
		painter->drawPath(m_shapeCachePath);

        // arrows
		if (isArrow && m_itemFlags & CF_Start_Arrow)
			drawArrow(painter, option, true, QLineF(line().p2(), line().p1()));

		if (isArrow && m_itemFlags & CF_End_Arrow)
			drawArrow(painter, option, false, line());
	}
	else // curve
	{
		painter->setBrush(Qt::NoBrush);
		painter->drawPath(m_shapeCachePath);

		// arrows
        if (isArrow && m_itemFlags & CF_Start_Arrow)
        {
            QLineF arrowLine = calculateArrowLine(m_shapeCachePath, true, QLineF(m_controlPos, line().p1()));
            drawArrow(painter, option, true, arrowLine);
        }

        if (isArrow && m_itemFlags & CF_End_Arrow)
        {
            QLineF arrowLine = calculateArrowLine(m_shapeCachePath, false, QLineF(m_controlPos, line().p2()));
            drawArrow(painter, option, false, arrowLine);
        }
    }
}


void CDirectEdge::updateLabelPosition()
{
	auto r = m_labelItem->boundingRect();
	int w = r.width();
	int h = r.height();
	m_labelItem->setTransformOriginPoint(w / 2, h / 2);

	if (isCircled())
	{
		m_labelItem->setPos(m_controlPoint.x() - w / 2, m_controlPoint.y() - boundingRect().height() / 2 - h);

		m_labelItem->setRotation(0);
	}
	else
	{
		m_labelItem->setPos(m_controlPoint.x() - w / 2, m_controlPoint.y() - h / 2);

		// update label rotation
		//qreal angle = 180 - line().angle();
		//if (angle > 90) angle -= 180;
		//else if (angle < -90) angle += 180;
		//qDebug() << angle;
		//m_labelItem->setRotation(angle);
	}
}


// callbacks 

void CDirectEdge::onParentGeometryChanged()
{
	// optimize: no update while restoring
	if (s_duringRestore)
		return;

	if (!m_firstNode || !m_lastNode)
		return;

	prepareGeometryChange();

	// update line position
	QPointF p1c = m_firstNode->pos();
	if (m_firstPortId.size() && m_firstNode->getPort(m_firstPortId))
		p1c = m_firstNode->getPort(m_firstPortId)->scenePos();

	QPointF p2c = m_lastNode->pos();
	if (m_lastPortId.size() && m_lastNode->getPort(m_lastPortId))
		p2c = m_lastNode->getPort(m_lastPortId)->scenePos();

	QPointF p1 = m_firstNode->getIntersectionPoint(QLineF(p1c, p2c), m_firstPortId);
	QPointF p2 = m_lastNode->getIntersectionPoint(QLineF(p2c, p1c), m_lastPortId);

	bool intersected = (!p1.isNull()) && (!p2.isNull());

	QLineF l(p1, p2);
	setLine(l);

	// update shape path
	m_shapeCachePath = QPainterPath();

	// circled connection 
	if (isCircled())
	{
		int nodeDiameter = m_firstNode->boundingRect().height();
		double nr = nodeDiameter /*/ 2*/;
		double r = nr + qAbs(m_bendFactor) * nr / 2;

		// left up point
		QPointF lp = p1c + QPointF(-r, -r);
		QPointF p1 = m_firstNode->getIntersectionPoint(QLineF(lp, p1c), m_firstPortId);

		// right up point
		QPointF rp = p2c + QPointF(r, -r);
		QPointF p2 = m_lastNode->getIntersectionPoint(QLineF(rp, p2c), m_lastPortId);

		// up point
		m_controlPos = (p1c + p2c) / 2 + QPointF(0, -r * 2);

		QLineF l(p1, p2);
		setLine(l);

		m_controlPoint = (lp + rp) / 2;

		m_shapeCachePath.moveTo(p1);
		m_shapeCachePath.cubicTo(lp, rp, p2);
	}
	else // not circled
	{
		m_shapeCachePath.moveTo(p1);

		// center
		m_controlPos = (p1c + p2c) / 2;

		if (m_bendFactor == 0)
		{
			m_shapeCachePath.lineTo(p2);
            m_controlPoint = (line().p1() + line().p2()) / 2;
		}
		else
		{
			QPointF t1 = m_controlPos;
			float posFactor = qAbs(m_bendFactor);

			bool bendDirection = (quint64(m_firstNode) > quint64(m_lastNode));
			if (m_bendFactor < 0)
				bendDirection = !bendDirection;

			QLineF f1(t1, p2c);
			f1.setAngle(bendDirection ? f1.angle() + 90 : f1.angle() - 90);
			f1.setLength(f1.length() * 0.2 * posFactor);

			m_controlPos = f1.p2();
			m_controlPoint = m_controlPos - (t1 - m_controlPos) * 0.33;

			m_shapeCachePath.cubicTo(m_controlPoint, m_controlPoint, p2);
		}

		auto len = line().length();
		auto fullLen = QLineF(p1c, p2c).length();
		//qDebug() << len << fullLen;

		// if no intersection or len == fullLen : drop the shape
		if (!intersected || qAbs(len - fullLen) < 5)
		{
			m_shapeCachePath = QPainterPath();
		}
	}

	QPainterPathStroker stroker;
	stroker.setWidth(6);
	m_selectionShapePath = stroker.createStroke(m_shapeCachePath);

	update();

	// update text label
	if (getScene() && getScene()->itemLabelsEnabled())
	{
		updateLabelPosition();
		updateLabelDecoration();
	}
}


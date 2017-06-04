/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/


#include "CItem.h"

#include <QGraphicsSceneMouseEvent>
#include <QMenu>


CItem::CItem()
{
	m_labelItem = NULL;

	// default item flags
	m_itemFlags = IF_DeleteAllowed | IF_FramelessSelection;
	m_internalStateFlags = IS_Attribute_Changed;
}

CItem::~CItem()
{
	CEditorScene *scene = getScene();
	if (scene)
		scene->onItemDestroyed(this);

	//if (m_labelItem)
	//	delete m_labelItem;
}


// IO

bool CItem::storeTo(QDataStream &out, quint64 version64) const
{
	if (version64 >= 2)
	{
		out << m_attributes;
	}

	if (version64 >= 4)
	{
		out << m_id;
	}

	return true;
}

bool CItem::restoreFrom(QDataStream &out, quint64 version64)
{
	if (!out.atEnd())
	{
		if (version64 >= 2)
		{
			out >> m_attributes;
		}
		else
			m_attributes.clear();

		if (version64 >= 4)
		{
			out >> m_id;
		}

		// ?
		updateCachedItems();

		return true;
	}

	return false;
}


// attributes

bool CItem::setAttribute(const QByteArray& attrId, const QVariant& v)
{
	setItemStateFlag(IS_Attribute_Changed);

	if (attrId == "id")
	{
		m_id = v.toString();
		return true;
	}

	// real attributes
	m_attributes[attrId] = v;

	return true;
}

bool CItem::removeAttribute(const QByteArray& attrId)
{
	return m_attributes.remove(attrId);
}

QVariant CItem::getAttribute(const QByteArray& attrId) const
{
	if (attrId == "id")
		return m_id;

	if (m_attributes.contains(attrId))
		return m_attributes[attrId];

	return getClassAttribute(attrId);
}

QSet<QByteArray> CItem::getVisibleAttributeIds(int flags) const
{
	QSet<QByteArray> result;

	if (flags == VF_ANY || flags == VF_TOOLTIP)
        result = getLocalAttributes().keys().toSet();

	auto scene = getScene();
	if (scene)
	{
		if (flags == VF_ANY || flags == VF_TOOLTIP)
			result += scene->getClassAttributes(classId(), true).keys().toSet();
		else
			result += scene->getVisibleClassAttributes(classId(), true);
	}

    return result;
}

QVariant CItem::getClassAttribute(const QByteArray& attrId) const
{
	auto scene = getScene();
	if (scene)
	{
		QByteArray lookId = classId();

		while (!lookId.isEmpty())
		{
			auto v = scene->getClassAttribute(lookId, attrId);
			if (v.isValid()) 
				return v;

			lookId = scene->getSuperClassId(lookId);
		}
	}

	return QVariant();
}

bool CItem::setDefaultId()
{
	if (m_id.isEmpty())
	{
		m_id = createNewId();
		return true;
	}

	return false;
}


// cloning

void CItem::copyDataFrom(CItem* from)
{
	m_itemFlags = from->m_itemFlags;
	
	// copy attrs
	m_attributes = from->m_attributes;

	updateCachedItems();
}


// painting

void CItem::drawLabel(QPainter *painter, const QStyleOptionGraphicsItem* /*option*/)
{
	return;
	/*

	if (m_label && !m_label->text().isEmpty())
	{
		QRectF r = getSceneItem()->boundingRect();
		QPointF p = labelOffset(r, m_label->localSize());

		QRectF labelRect(getSceneItem()->mapToScene(p), m_label->localSize());
		if (getScene()->checkLabelRegion(labelRect))
		{
			painter->setBrush(Qt::NoBrush);

			QColor c = getAttribute("label.color").value<QColor>();
			if (c.isValid())
				painter->setPen(c);

			bool ok = false;
			int s = getAttribute("label.size").toInt(&ok);
			if (ok && s > 0)
			{
				QFont f(painter->font());
				f.setPointSize(s);
				painter->setFont(f);
			}

			//painter->drawStaticText(p, *m_label);
			painter->drawText(p.x(), p.y(), 1, 1,
				Qt::TextDontClip,
				m_label->text());
		}
	}
	*/
}


void CItem::updateTextInfo()
{
	if (!(m_internalStateFlags & IS_Attribute_Changed) && 
		!(getScene()->itemLabelsEnabled()) &&
		!(getScene()->itemLabelsNeedUpdate())
	)
		return;

	resetItemStateFlag(IS_Attribute_Changed);

	QString labelToShow;
	auto idsToShow = getVisibleAttributeIds(CItem::VF_LABEL);
	for (const QByteArray& id : idsToShow)
	{
		QString text = Utils::variantToText(getAttribute(id));
		if (labelToShow.size()) labelToShow += "\n";
		labelToShow += QString("%1: \t%2").arg(QString(id)).arg(text);
	}
	
	setLabelText(labelToShow);
}


void CItem::setLabelText(const QString& text)
{
	if (m_labelItem)
		m_labelItem->setPlainText(text);
}


void CItem::showLabel(bool on)
{
	if (m_labelItem)
		m_labelItem->setVisible(on);
}


QRectF CItem::getSceneLabelRect() const 
{
	if (!m_labelItem)
		return QRectF();
	
	return m_labelItem->mapRectToScene(m_labelItem->boundingRect());
}


// callbacks

void CItem::onHoverEnter(QGraphicsItem* sceneItem, QGraphicsSceneHoverEvent*)
{
	// update tooltip
	QString tooltipToShow;

	auto idsToShow = getVisibleAttributeIds(CItem::VF_TOOLTIP);
	for (const QByteArray& id : idsToShow)
	{
		QString text = Utils::variantToText(getAttribute(id));
		if (tooltipToShow.size()) tooltipToShow += "\n";
		tooltipToShow += QString("%1: %2").arg(QString(id)).arg(text);
	}

	sceneItem->setToolTip(tooltipToShow);
}

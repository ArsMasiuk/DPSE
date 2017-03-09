#include "Utils.h"

#include <QPoint>
#include <QPointF>


QString Utils::variantToText(const QVariant& v)
{
	switch (v.type())
	{
	case QVariant::Point:
		return QString("%1;%2").arg(v.toPoint().x()).arg(v.toPoint().y());

	case QVariant::PointF:
		return QString("%1;%2").arg(v.toPointF().x()).arg(v.toPointF().y());

	case QVariant::Bool:
		return v.toBool() ? "true" : "false";

	//case QVariant::Double:
	//	return QString::number(v.toDouble());

	//case QVariant::UInt:
	//	return QString::number(v.toUInt());

	//case QVariant::Int:
	//	return QString::number(v.toInt());

	//case QVariant::ULongLong:
	//	return QString::number(v.toULongLong());

	//case QVariant::LongLong:
	//	return QString::number(v.toLongLong());

	default:;
	}

	return v.toString();
}


/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CAttribute.h"

CAttribute::CAttribute()
    : isVirtual(false)
{
}

CAttribute::CAttribute(const QByteArray& classId, const QByteArray& attrId, const QString& attrName, const QVariant& defaultValue)
    : isVirtual(false)
{
	this->classId = classId;
	this->id = attrId;
	this->name = attrName;
	this->defaultValue = defaultValue;
}

bool CAttribute::storeTo(QDataStream & out, quint64 /*version64*/) const
{
    out << id << classId << name << defaultValue << true << isVirtual;
	return true;
}

bool CAttribute::restoreFrom(QDataStream & out, quint64 /*version64*/)
{
    static bool dummy;
    out >> id >> classId >> name >> defaultValue >> dummy >> isVirtual;
	return true;
}

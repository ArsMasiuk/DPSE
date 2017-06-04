/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CAttribute.h"


// attributes

CAttribute::CAttribute()
    : isVirtual(false),
	valueType(QVariant::String)
{
}

CAttribute::CAttribute(
	const QByteArray& attrId, const QString& attrName, 
	const QVariant& defaultValue, const RangeVariant& range)
	: isVirtual(false),
	valueType(defaultValue.type())
{
	this->id = attrId;
	this->name = attrName;
	this->defaultValue = defaultValue;

	valueRange = range;
}

bool CAttribute::storeTo(QDataStream& out, quint64 /*version64*/) const
{
    out << id << name << defaultValue << true << isVirtual;

	return true;
}

bool CAttribute::restoreFrom(QDataStream& out, quint64 /*version64*/)
{
    static bool dummy;
    out >> id >> name >> defaultValue >> dummy >> isVirtual;

	valueType = defaultValue.type();

	return true;
}


// attribute constrains

AttributeConstrainsMap CAttributeConstrains::s_constrains;


CAttributeConstrains::~CAttributeConstrains()
{
	// dummy
}


CAttributeConstrains* CAttributeConstrains::getClassConstrains(const QByteArray& classId, const QByteArray& attrId)
{
	ClassAttrIndex index(classId, attrId);

	if (s_constrains.contains(index))
		return s_constrains[index];
	else
		return NULL;
}


void CAttributeConstrains::setClassConstrains(const QByteArray& classId, const QByteArray& attrId, CAttributeConstrains* cptr)
{
	ClassAttrIndex index(classId, attrId);

	if (cptr)
		s_constrains[index] = cptr;
	else
		s_constrains.remove(index);
}

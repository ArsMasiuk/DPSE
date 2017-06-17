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
	const QVariant& defaultValue)
	: isVirtual(false),
	valueType(defaultValue.type())
{
	this->id = attrId;
	this->name = attrName;
	this->defaultValue = defaultValue;
}

bool CAttribute::storeTo(QDataStream& out, quint64 /*version64*/) const
{
    out << id << name << defaultValue << true << isVirtual;

	return true;
}

bool CAttribute::restoreFrom(QDataStream& out, quint64 version64)
{
	out >> id;

    static bool dummy;

	if (version64 < 6)
		out >> classId;	// dummy value

    out >> name >> defaultValue >> dummy >> isVirtual;

	// size must be converted
	if (version64 < 7)
	{
		if (id == "size")
			defaultValue = QSizeF(defaultValue.toDouble(), defaultValue.toDouble());
	}

	valueType = defaultValue.type();

	return true;
}


// attribute constrains

CAttributeConstrains::~CAttributeConstrains()
{
	// dummy
}


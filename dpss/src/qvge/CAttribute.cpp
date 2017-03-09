#include "CAttribute.h"

CAttribute::CAttribute()
	: isVisible(false),
	isVirtual(false)
{

}

CAttribute::CAttribute(const QByteArray& classId, const QByteArray& attrId, const QString& attrName, const QVariant& defaultValue, bool visible)
	: isVisible(visible),
	isVirtual(false)
{
	this->classId = classId;
	this->id = attrId;
	this->name = attrName;
	this->defaultValue = defaultValue;
}

bool CAttribute::storeTo(QDataStream & out, quint64 version64) const
{
	out << id << classId << name << defaultValue << isVisible << isVirtual;
	return true;
}

bool CAttribute::restoreFrom(QDataStream & out, quint64 version64)
{
	out >> id >> classId >> name >> defaultValue >> isVisible >> isVirtual;
	return true;
}

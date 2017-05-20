/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QByteArray>
#include <QString>
#include <QVariant>
#include <QMap>
#include <QDataStream>


struct CAttribute
{
	CAttribute();
    CAttribute(const QByteArray& classId, const QByteArray& attrId, const QString& attrName, const QVariant& defaultValue);

	QByteArray id, classId;
	QString name;
	QVariant defaultValue;
	bool isVirtual;	// x,y,label,color etc.

	int valueType;

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);
};

typedef QMap<QByteArray, CAttribute> AttributesMap;		// <attr.id, attr>

typedef QMap<QByteArray, AttributesMap> ClassAttributesMap;		// <class.id, attr.map>

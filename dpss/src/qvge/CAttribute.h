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
#include <QList>
#include <QIcon>


// attribute class

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


// attribute constrains

typedef QPair<QByteArray, QByteArray> ClassAttrIndex;

struct CAttributeConstrains;
typedef QMap<ClassAttrIndex, CAttributeConstrains*> AttributeConstrainsMap;

struct CAttributeConstrains
{
	virtual ~CAttributeConstrains();

	static CAttributeConstrains* getClassConstrains(const QByteArray& classId, const QByteArray& attrId);
	static void setClassConstrains(const QByteArray& classId, const QByteArray& attrId, CAttributeConstrains* cptr);

private:
	static AttributeConstrainsMap s_constrains;
};


// list of id items

typedef QList<QIcon> IconsList;

struct CAttributeConstrainsList: public CAttributeConstrains
{
	virtual ~CAttributeConstrainsList() {}

	QStringList names;
	QStringList ids;
	IconsList icons;
};


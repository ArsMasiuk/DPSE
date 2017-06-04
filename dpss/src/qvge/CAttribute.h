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


// type constrains

union RangeVariant
{
	int rangeType = 0;

	RangeVariant(): rangeType(0)
	{}

	// real value range
	RangeVariant(double minV, double maxV, int dec) : rangeType(QVariant::Double)
	{
		real.minValue = minV;
		real.maxValue = maxV;
		real.decPoints = dec;
	}

	struct RealRange
	{
		int rangeType = QVariant::Double;
		double minValue = std::numeric_limits<double>::lowest();
		double maxValue = std::numeric_limits<double>::max();
		int decPoints = 4;
	} real;

	RealRange getRealRange() const
	{
		return rangeType == QVariant::Double ? real : RealRange();
	}
};


// attribute class

struct CAttribute
{
	CAttribute();
    CAttribute(const QByteArray& attrId, const QString& attrName, const QVariant& defaultValue,
		const RangeVariant& range = RangeVariant());

	QByteArray id;
	QString name;
	QVariant defaultValue;
	bool isVirtual;	// x,y,label,color etc.

	int valueType;
	RangeVariant valueRange;

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);

	// dummy member
	QByteArray classId;
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


/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include "CConnection.h"


class CControlPoint;


class CPolylineConnection : public CConnection
{
public:
	typedef CConnection Super;

	CPolylineConnection(QGraphicsItem *parent = Q_NULLPTR);

	void setPoints(const QList<QPointF> &points);
	bool insertPointAt(const QPointF &pos);

	// reimp
	static QByteArray factoryId() { return "CPolylineConnection"; }
	virtual QByteArray typeId() const { return this->factoryId(); }
	virtual QByteArray classId() const { return "edge"; }
	virtual QByteArray superClassId() const { return Super::classId(); }

	virtual CItem* create() const { return new CPolylineConnection(parentItem()); }
	CConnection* clone();

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);

	// mousing
	virtual bool onDoubleClickDrag(QGraphicsSceneMouseEvent *mouseEvent, const QPointF &clickPos);
	virtual void onControlPointMoved(CControlPoint* controlPoint, const QPointF& pos);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	// selection
	virtual void onItemSelected(bool state);

protected:
	// painting
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
	virtual void updateLabelPosition();

	// callbacks 
	virtual void onParentGeometryChanged();

private:
	void dropControlPoints();
	void createControlPoints();
	void updateShapeFromPoints();

private:
	// data model
	QList<QPointF> m_polyPoints;

	// visual control points
	QList<CControlPoint*> m_controlPoints;
};

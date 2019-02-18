/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2019 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#pragma once

#include <QCursor> 
#include <QPainter>
#include <QRectF>
#include <QPointF>

#include "ISceneEditController.h"

class CEditorScene;


class CTransformRect: public QObject, public ISceneEditController
{
	Q_OBJECT

public:
	CTransformRect();
	~CTransformRect();

	// ISceneEditController
	virtual void onActivated(CEditorScene& scene);
	virtual void onDeactivated(CEditorScene& scene) {};
	virtual void draw(CEditorScene& scene, QPainter *painter, const QRectF &r);

protected:
	struct ControlPoint
	{
		QPointF pos;
		QCursor cursor;
	};

	ControlPoint m_points[8];
};


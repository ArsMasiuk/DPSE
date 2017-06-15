/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CEditorView.h"
#include "CEditorScene.h"

#include <QGLWidget> 
#include <QOpenGLWidget> 
#include <QMouseEvent> 
#include <QTimer> 
#include <QDebug> 


CEditorView::CEditorView(CEditorScene *scene, QWidget *parent)
	: Super(parent),
	m_menuModeTmp(Qt::PreventContextMenu)
{
	//auto glw = new QOpenGLWidget();
	//glw->setUpdateBehavior(QOpenGLWidget::PartialUpdate);
	//QSurfaceFormat sf;
	//sf.setRenderableType(QSurfaceFormat::OpenGLES);
	//glw->setFormat(sf);
	//setViewport(glw);

	//auto glw = new QGLWidget();
	//setViewport(glw);

	setAttribute(Qt::WA_TranslucentBackground, false);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setScene(scene);

	setDragMode(RubberBandDrag);

	setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setOptimizationFlags(DontSavePainterState);
	setOptimizationFlags(DontClipPainter);
	setOptimizationFlags(DontAdjustForAntialiasing);

	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

CEditorView::~CEditorView()
{

}


// reimp

void CEditorView::mousePressEvent(QMouseEvent *e)
{
	Super::mousePressEvent(e);
}

void CEditorView::mouseMoveEvent(QMouseEvent *e)
{
	// enable RMB pan
	if (e->buttons() == Qt::RightButton)
	{
		if (dragMode() != ScrollHandDrag)
		{
			m_menuModeTmp = contextMenuPolicy();
			setContextMenuPolicy(Qt::PreventContextMenu);

			setDragMode(ScrollHandDrag);

			QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
			Super::mousePressEvent(&fake);
		}
	}

	Super::mouseMoveEvent(e);
}


void CEditorView::mouseReleaseEvent(QMouseEvent *e)
{
	// disabel RMB pan
	if (e->button() == Qt::RightButton && !e->buttons() && (dragMode() == ScrollHandDrag))
	{
		QMouseEvent fake(e->type(), e->pos(), Qt::LeftButton, Qt::LeftButton, e->modifiers());
		Super::mouseReleaseEvent(&fake);

		setDragMode(RubberBandDrag);

		QTimer::singleShot(100, this, SLOT(restoreContextMenu()));
	}
	else
	{
		Super::mouseReleaseEvent(e);
	}
}


void CEditorView::restoreContextMenu()
{
	setContextMenuPolicy(m_menuModeTmp);
}

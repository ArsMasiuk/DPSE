/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016-2018 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#include "CEditorView.h"
#include "CEditorScene.h"

//#include <QGLWidget>
#include <QOpenGLWidget> 
#include <QMouseEvent> 
#include <QTimer> 
#include <QDebug> 


CEditorView::CEditorView(QWidget *parent)
	: Super(parent),
	m_menuModeTmp(Qt::PreventContextMenu),
	m_currentZoom(1.0)
{
    setAttribute(Qt::WA_TranslucentBackground, false);
	setViewportUpdateMode(BoundingRectViewportUpdate);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	setDragMode(RubberBandDrag);

    setRenderHint(QPainter::Antialiasing);
	setOptimizationFlags(DontSavePainterState);
    setOptimizationFlags(DontAdjustForAntialiasing);
}

CEditorView::CEditorView(CEditorScene *scene, QWidget *parent): CEditorView(parent)
{
    //auto glw = new QOpenGLWidget();

    //setViewport(glw);

	//auto glw = new QGLWidget();
	//setViewport(glw);

	setScene(scene);
}

CEditorView::~CEditorView()
{
}


// zoom

void CEditorView::zoomTo(double target)
{
	QTransform mat;
	mat.scale(target, target);
	setTransform(mat);
	
	m_currentZoom = target;

	Q_EMIT scaleChanged(m_currentZoom);
}


void CEditorView::zoomBy(double factor)
{
	double target = m_currentZoom * factor;

	zoomTo(target);
}


void CEditorView::fitToView()
{
	fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);

	m_currentZoom = matrix().m11();

	Q_EMIT scaleChanged(m_currentZoom);
}


void CEditorView::fitSelectedToView()
{
	auto items = scene()->selectedItems();
	if (items.isEmpty())
		return;

	QRectF r;
	for (const auto item : items)
	{
		r |= item->sceneBoundingRect();
	}

	fitInView(r, Qt::KeepAspectRatio);

	m_currentZoom = matrix().m11();

	Q_EMIT scaleChanged(m_currentZoom);
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

			m_interactiveTmp = isInteractive();
			setInteractive(false);

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

		setInteractive(m_interactiveTmp);

		QTimer::singleShot(100, this, SLOT(restoreContextMenu()));
	}
	else
	{
		Super::mouseReleaseEvent(e);
	}
}


void CEditorView::wheelEvent(QWheelEvent *e)
{
	// original taken from
	// http://blog.automaton2000.com/2014/04/mouse-centered-zooming-in-qgraphicsview.html

	if ((e->modifiers() & Qt::ControlModifier) == Qt::ControlModifier
		&& e->angleDelta().x() == 0) 
	{
		QPoint  pos = e->pos();
		QPointF posf = this->mapToScene(pos);

		double by = 1.0;
		double angle = e->angleDelta().y();

		if (angle > 0) { by = 1 + (angle / 360 * 0.5); }
		else 
			if (angle < 0) { by = 1 - (-angle / 360 * 0.5); }

		//this->scale(by, by);
		zoomBy(by);

		double w = this->viewport()->width();
		double h = this->viewport()->height();

		double wf = this->mapToScene(QPoint(w - 1, 0)).x() - this->mapToScene(QPoint(0, 0)).x();
		double hf = this->mapToScene(QPoint(0, h - 1)).y() - this->mapToScene(QPoint(0, 0)).y();

		double lf = posf.x() - pos.x() * wf / w;
		double tf = posf.y() - pos.y() * hf / h;

		/* try to set viewport properly */
		this->ensureVisible(lf, tf, wf, hf, 0, 0);

		QPointF newPos = this->mapToScene(pos);

		/* readjust according to the still remaining offset/drift
		* I don't know how to do this any other way */
		this->ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)), 0, 0);

		e->accept();
	}

	if ((e->modifiers() & Qt::ControlModifier) != Qt::ControlModifier) {
		/* no scrolling while control is held */
		Super::wheelEvent(e);
	}
}


void CEditorView::restoreContextMenu()
{
	setContextMenuPolicy(m_menuModeTmp);
}

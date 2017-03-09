#include "CEditorView.h"
#include "CEditorScene.h"

#include <QOpenGLWidget> 
#include <QMouseEvent> 
#include <QDebug> 


CEditorView::CEditorView(CEditorScene *scene, QWidget *parent)
	: Super(parent)
{
	//setViewport(new QOpenGLWidget());
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setScene(scene);

	setDragMode(RubberBandDrag);
	setRenderHint(QPainter::Antialiasing);
	setRenderHint(QPainter::HighQualityAntialiasing);
	setOptimizationFlags(DontSavePainterState);
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
	}
	else
	{
		Super::mouseReleaseEvent(e);
	}
}

#include "CDefaultInteractor.h"
#include "CEditorView.h"
#include "CNode.h"

#include <QMouseEvent>


CDefaultInteractor::CDefaultInteractor()
	: m_dragNode(NULL)
{
}

CDefaultInteractor::~CDefaultInteractor()
{
}


// callbacks

void CDefaultInteractor::onStarted(CEditorView * view, CEditorScene * scene)
{
	view->setDragMode(QGraphicsView::RubberBandDrag);
}

void CDefaultInteractor::onFinished(CEditorView * view, CEditorScene * scene)
{
	view->setDragMode(QGraphicsView::NoDrag);
}

bool CDefaultInteractor::onMousePressed(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	QPoint viewPos = event->pos();
	QGraphicsItem *item = view->itemAt(viewPos);
	if (item)   // clicked on a real item
	{
		CNode* node = dynamic_cast<CNode*>(item);
		if (node)
		{
			if (node->onMousePressed())
			{
				m_dragNode = node;
			}
			else
			{
				m_dragNode = NULL;
				return false;	// no drag
			}
		}
	}

	return true;
}

bool CDefaultInteractor::onMouseMoved(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	return true;
}

bool CDefaultInteractor::onMouseReleased(CEditorView * view, CEditorScene * scene, QMouseEvent * event)
{
	m_dragNode = NULL;

	return true;
}

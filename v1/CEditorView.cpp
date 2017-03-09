#include "CEditorView.h"
#include "CEditorScene.h"
#include "CDefaultInteractor.h"

#include <QMouseEvent>


CEditorView::CEditorView(QWidget *parent): QGraphicsView(parent), m_interactorPtr(NULL)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}


void CEditorView::setInteractor(CDefaultInteractor * interactor)
{
	if (m_interactorPtr == interactor)
		return;

	CEditorScene *scenePtr = dynamic_cast<CEditorScene*>(scene());

	if (m_interactorPtr && scenePtr)
		m_interactorPtr->onFinished(this, scenePtr);

	m_interactorPtr = interactor;

	if (m_interactorPtr && scenePtr)
		m_interactorPtr->onStarted(this, scenePtr);
}

void CEditorView::mousePressEvent(QMouseEvent * event)
{
	bool callSuper = true;

	CEditorScene *scenePtr = dynamic_cast<CEditorScene*>(scene());

	if (m_interactorPtr && scenePtr)
		callSuper = m_interactorPtr->onMousePressed(this, scenePtr, event);

	if (callSuper)
		Super::mousePressEvent(event);
}

void CEditorView::mouseMoveEvent(QMouseEvent * event)
{
	bool callSuper = true;
	
	CEditorScene *scenePtr = dynamic_cast<CEditorScene*>(scene());

	if (m_interactorPtr && scenePtr)
		callSuper = m_interactorPtr->onMouseMoved(this, scenePtr, event);

	if (callSuper)
		Super::mouseMoveEvent(event);
}

void CEditorView::mouseReleaseEvent(QMouseEvent * event)
{
	bool callSuper = true;

	CEditorScene *scenePtr = dynamic_cast<CEditorScene*>(scene());

	if (m_interactorPtr && scenePtr)
		callSuper = m_interactorPtr->onMouseReleased(this, scenePtr, event);

	if (callSuper)
		Super::mouseReleaseEvent(event);
}



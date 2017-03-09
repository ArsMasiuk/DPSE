#pragma once


class CEditorView;
class CEditorScene;
class CNode;

class QMouseEvent;


class CDefaultInteractor
{
public:
	CDefaultInteractor();
	~CDefaultInteractor();

	// callbacks
	virtual void onStarted(CEditorView * view, CEditorScene * scene);
	virtual void onFinished(CEditorView * view, CEditorScene * scene);

	virtual bool onMousePressed(CEditorView *view, CEditorScene *scene, QMouseEvent *event);
	virtual bool onMouseMoved(CEditorView *view, CEditorScene *scene, QMouseEvent *event);
	virtual bool onMouseReleased(CEditorView *view, CEditorScene *scene, QMouseEvent *event);

protected:
	CNode * m_dragNode;
};


#pragma once

#include "CDefaultInteractor.h"


class CNode;
class CConnection;


class CBranchInteractor :
	public CDefaultInteractor
{
public:
	typedef CDefaultInteractor Super;

	CBranchInteractor();
	virtual ~CBranchInteractor();

	
	// callbacks
	virtual void onStarted(CEditorView * view, CEditorScene * scene);
	virtual void onFinished(CEditorView * view, CEditorScene * scene);

	virtual bool onMousePressed(CEditorView *view, CEditorScene *scene, QMouseEvent *event);
	virtual bool onMouseMoved(CEditorView *view, CEditorScene *scene, QMouseEvent *event);
	virtual bool onMouseReleased(CEditorView *view, CEditorScene *scene, QMouseEvent *event);

protected:
	void doCleanup(CEditorScene *scene);

private:
	CNode *m_startNode, *m_endNode;
	bool m_startRealNode;
	CConnection *m_connection;
};


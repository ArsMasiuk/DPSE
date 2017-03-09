#ifndef CEDITORVIEW_H
#define CEDITORVIEW_H

#include <QGraphicsView>

class CEditorScene;

class CEditorView : public QGraphicsView
{
public:
	typedef QGraphicsView Super;

	CEditorView(CEditorScene *scene, QWidget *parent = NULL);
	~CEditorView();

	// reimp
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

private:
	
};

#endif // CEDITORVIEW_H

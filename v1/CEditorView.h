#ifndef CBRANCHVIEW_H
#define CBRANCHVIEW_H

#include <QGraphicsView>


class CDefaultInteractor;


class CEditorView : public QGraphicsView
{
public:
	typedef QGraphicsView Super;

    CEditorView(QWidget *parent);

	void setInteractor(CDefaultInteractor *interactor);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

protected:
	CDefaultInteractor* m_interactorPtr;
};

#endif // CBRANCHVIEW_H

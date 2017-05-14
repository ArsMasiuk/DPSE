/*
This file is a part of
QVGE - Qt Visual Graph Editor

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CEDITORVIEW_H
#define CEDITORVIEW_H

#include <QGraphicsView>

class CEditorScene;

class CEditorView : public QGraphicsView
{
	Q_OBJECT
public:
	typedef QGraphicsView Super;

	CEditorView(CEditorScene *scene, QWidget *parent = NULL);
	~CEditorView();

	// reimp
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

private:
	Qt::ContextMenuPolicy m_menuModeTmp;

private slots:
	void restoreContextMenu();
};

#endif // CEDITORVIEW_H

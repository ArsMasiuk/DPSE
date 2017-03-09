#pragma once

#include "CEditorScene.h"

class CNode;
class CConnection;


class CNodeEditorScene : public CEditorScene
{
	Q_OBJECT

public:
	typedef CEditorScene Super;

	CNodeEditorScene(QObject *parent);

	// reimp
	virtual void initialize();

	// operations
	bool startNewConnection(const QPointF& pos);
	void cancel(const QPointF& pos = QPointF());

	// factorizations
	virtual CNode* createNewNode() const;
	virtual CConnection* createNewConnection() const;

	CConnection* activateConnectionFactory(const QByteArray& factoryId);

public Q_SLOTS:
	virtual void onActionUnlink();
	virtual void onActionNodeColor();
	virtual void onActionEdgeColor();
	virtual void onActionEdgeReverse();

protected:
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void keyPressEvent(QKeyEvent *keyEvent);

	// reimp
	virtual bool populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems);

protected:
	CNode *m_startNode, *m_endNode;
	CConnection *m_connection;
	bool m_realStart;

	CConnection *m_activeConnectionFactory;

	enum InternState {
		IS_None, IS_Creating, IS_Finishing, IS_Cancelling
	};
	InternState m_state;
};



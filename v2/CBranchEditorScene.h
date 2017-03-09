#ifndef CBRANCHEDITORSCENE_H
#define CBRANCHEDITORSCENE_H

#include <QMenu>

#include "CEditorScene.h"
#include "CNode.h"
#include "CDragController.h"
//#include "CNodeDragController.h"
#include "CNodeEditDragController.h"


enum BranchEditorMode
{
	EM_SELECT,
	EM_ADD_BRANCH,
	EM_ADD_FAN
};


class CBranchNode : public CRoundNode
{
public:
	typedef CRoundNode Super;

	CBranchNode(QGraphicsItem *parent = Q_NULLPTR);

	virtual CItem* create() const
	{
		return new CBranchNode(parentItem());
	}
};


class CBranchEditorScene : public CEditorScene
{
	Q_OBJECT

public:
	typedef CEditorScene Super;

	CBranchEditorScene(QObject *parent);
	~CBranchEditorScene();

	BranchEditorMode mode() const { return m_mode;  }

	// serialization 
	virtual CItem* createItemOfType(const QByteArray& tid) const;

public Q_SLOTS:
	void setSelectionMode();
	void setBranchesMode();
	void setFansMode();

protected Q_SLOTS:
	void doUnlinkNode();
	void doDeleteNode();

protected:
	virtual void onItemPressedRight(CItem *citem, QGraphicsSceneMouseEvent *mouseEvent);

private:
	BranchEditorMode m_mode;
	CDragController m_dragController;
	//CNodeDragController m_nodeDragController;
	CNodeEditDragController m_nodeDragController;

	QMenu m_nodeMenu;
	QAction *m_unlinkAction;

	CNode *m_menuNode;
};


#endif // CBRANCHEDITORSCENE_H

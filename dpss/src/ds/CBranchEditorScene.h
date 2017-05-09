/*
This file is a part of
DPSE - Distributed Parallel Simulation Environment

(c) 2016 Ars L. Masiuk (ars.masiuk@gmail.com)

It can be used freely, maintaining the information above.
*/

#ifndef CBRANCHEDITORSCENE_H
#define CBRANCHEDITORSCENE_H

#include <QMenu>

#include "qvge/CNodeEditorScene.h"
#include "qvge/CNode.h"
#include "qvge/CConnection.h"


enum BranchEditorMode
{
	EM_ADD_BRANCH,
	EM_ADD_FAN
};


enum AttrIds
{
	LINK_L = 1000,
	LINK_F1,
	LINK_F2,
	LINK_R,
	LINK_H,
	LINK_Q,
	LINK_MODEL
};


class CBranchNode : public CNode
{
public:
	typedef CNode Super;

	CBranchNode(QGraphicsItem *parent = Q_NULLPTR);
	virtual ~CBranchNode() {}
	
	static QByteArray factoryId()		{ return "CBranchNode"; }
	virtual QByteArray typeId() const	{ return this->factoryId(); }

	virtual QString createNewId() const;
	virtual CItem* create() const		{ return new CBranchNode(parentItem());	}
};


class CFanNode : public CBranchNode
{
public:
    typedef CBranchNode Super;

	CFanNode(QGraphicsItem *parent = Q_NULLPTR);

	static QByteArray factoryId()		{ return "CFanNode"; }
	virtual QByteArray typeId() const	{ return this->factoryId(); }
    virtual QByteArray classId() const	{ return "fan"; }
    virtual QByteArray superClassId() const { return Super::classId(); }

	virtual QString createNewId() const;
	virtual CItem* create() const		{ return new CFanNode(parentItem()); }

	virtual QList<CNode*> unlink();

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};


class CBranchConnection : public CConnection
{
public:
	typedef CConnection Super;

	CBranchConnection(QGraphicsItem *parent = Q_NULLPTR);
	virtual ~CBranchConnection() {}

	static QByteArray factoryId()		{ return "CBranchConnection"; }
	virtual QByteArray typeId() const	{ return this->factoryId(); }

	virtual CItem* create() const		{ return new CBranchConnection(parentItem()); }

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
};


class CBranchEditorScene : public CNodeEditorScene
{
	Q_OBJECT

public:
	typedef CNodeEditorScene Super;

	CBranchEditorScene(QObject *parent);
	~CBranchEditorScene();

	virtual void initialize();

	BranchEditorMode mode() const { return m_mode;  }

public Q_SLOTS:
	void setBranchesMode();
	void setFansMode();

protected:
	virtual bool populateMenu(QMenu& menu, QGraphicsItem* item, const QList<QGraphicsItem*>& selectedItems);

	// reimp
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
	virtual void keyPressEvent(QKeyEvent *keyEvent);

private:
	BranchEditorMode m_mode;
};


#endif // CBRANCHEDITORSCENE_H

#ifndef CITEM_H
#define CITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "CEditorScene.h"
#include "CTextLabel.h"
#include "Properties.h"
#include "Utils.h"


enum ItemFlags
{
	IF_FramelessSelection = 1,
	IF_DeleteAllowed = 2,
	IF_LastFlag = 4
};


enum ItemStateFlags
{
	IS_Normal = 0,
	IS_Selected = 1,
	IS_Hover = 2,
	IS_Drag_Accepted = 4,
	IS_Drag_Rejected = 8,
	IS_Attribute_Changed = 16
};


enum ItemDragTestResult
{
	Rejected,
	Accepted,
	Ignored
};


class CItem
{
public:
	CItem();
	virtual ~CItem();

	int itemFlags() const		{ return m_itemFlags;	}
	void setItemFlags(int f)	{ m_itemFlags = f;		}
	void setItemFlag(int f)		{ m_itemFlags |= f;		}
	void resetItemFlag(int f)	{ m_itemFlags &= ~f;	}

	int itemStateFlags() const		{ return m_internalStateFlags; }
	void setItemStateFlag(int f)	{ m_internalStateFlags |= f; }
	void resetItemStateFlag(int f)	{ m_internalStateFlags &= ~f; }

	// to be reimplemented
	static QByteArray factoryId()		{ return "CItem"; }
	virtual QByteArray typeId() const	{ return this->factoryId(); }
    virtual QString createNewId() const { return QString::number((quint64)this); }
	virtual bool setDefaultId();
	
	// attributes
	virtual bool setAttribute(const QByteArray& attrId, const QVariant& v);
	virtual QVariant getAttribute(const QByteArray& attrId) const;
	virtual QVariant getClassAttribute(const QByteArray& attrId) const;
    virtual QByteArray classId() const			{ return "item"; }
    virtual QByteArray superClassId() const		{ return QByteArray(); }

	enum VisibleFlags { VF_ANY = 0, VF_LABEL = 1, VF_TOOLTIP = 2 };
    virtual QSet<QByteArray> getVisibleAttributeIds(int flags) const;

	const QMap<QByteArray, QVariant>& getLocalAttributes() const { 
		return m_attributes; 
	}

	QGraphicsItem* getSceneItem() const {
		return dynamic_cast<QGraphicsItem*>((CItem*)this);
	}

	CEditorScene* getScene() const { 
		QGraphicsItem* sceneItem = getSceneItem();
		if (sceneItem)
			return dynamic_cast<CEditorScene*>(sceneItem->scene());

		return NULL;
	}

	// painting
	virtual void drawLabel(QPainter *painter, const QStyleOptionGraphicsItem *option);
	virtual QPointF labelOffset(const QRectF& itemRect, const QSizeF& labelSize) const;

	// serialization 
	virtual bool storeTo(QDataStream& out, quint64 version64) const;
	virtual bool restoreFrom(QDataStream& out, quint64 version64);

    typedef QMap<quint64, CItem*> CItemLinkMap;
	virtual bool linkAfterRestore(const CItemLinkMap& /*idToItem*/) { return true; }

	// returns new item of this class
	virtual CItem* clone() = 0;
	virtual CItem* create() const = 0;

	// callbacks
	virtual void onItemMoved() {}
	virtual void onHoverEnter() {}
	virtual void onHoverLeave() {}
	virtual void onDraggedOver(const QSet<CItem*>& /*acceptedItems*/, const QSet<CItem*>& /*rejectedItems*/) {}
	virtual void onDroppedOn(const QSet<CItem*>& /*acceptedItems*/, const QSet<CItem*>& /*rejectedItems*/) {}

	// call from drag event
	virtual ItemDragTestResult acceptDragFromItem(QGraphicsItem* /*draggedItem*/) { return Accepted; }
	virtual void leaveDragFromItem(QGraphicsItem* /*draggedItem*/) {}

protected:
	virtual void copyDataFrom(CItem* from);

	virtual void updateTextInfo();
	virtual void setLabelText(const QString& text);

protected:
	int m_itemFlags;
	int m_internalStateFlags;
	QMap<QByteArray, QVariant> m_attributes;
	QString m_id;
	CTextLabel *m_label;
};


#endif // CITEM_H
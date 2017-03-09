#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QGraphicsLineItem>

#include "CNode.h"


class CConnection : public TItem<QGraphicsLineItem, CItem>
{
public:
	typedef TItem<QGraphicsLineItem, CItem> Super;

    CConnection(QGraphicsItem *parent = Q_NULLPTR);
	virtual ~CConnection();

	// reimp
	virtual QByteArray typeId() const { return "CConnection"; }

	virtual CItem* create() const
	{
		return new CConnection(parentItem());
	}

	// serialization 
	virtual bool storeTo(QDataStream& out) const;
	virtual bool restoreFrom(QDataStream& out);
	virtual bool linkAfterRestore(const CItemLinkMap& idToItem);

	// public
    void setFirstNode(CNode *node);
    void setLastNode(CNode *node);

	void reattach(CNode *oldNode, CNode *newNode);

	CConnection* clone();

	bool isValid() const { return m_firstNode != NULL && m_lastNode != NULL; }
	bool isCircled() const { return isValid() && m_firstNode == m_lastNode; }

    // callback
    void onNodeMoved(CNode *node);
	void onNodeDetach(CNode *node);

protected:
    CNode *m_firstNode, *m_lastNode;
};

#endif // CCONNECTION_H

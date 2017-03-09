#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QGraphicsLineItem>

#include "CNode.h"


class CConnection : public QGraphicsLineItem
{
public:
    CConnection(QGraphicsItem *parent = Q_NULLPTR);

    void setFirstNode(CNode *node);
    void setLastNode(CNode *node);

    // callback
    void onNodeMoved(CNode *node);
	void onNodeDetach(CNode *node);

protected:
    CNode *m_firstNode, *m_lastNode;
};

#endif // CCONNECTION_H

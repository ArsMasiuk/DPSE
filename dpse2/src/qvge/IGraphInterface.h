#ifndef IGRAPHINTERFACE_H
#define IGRAPHINTERFACE_H

#include <QString>
#include <QByteArray>
#include <QVariant>

class CConnection;
class CNode;


class IGraphInterface
{
public:
    virtual CNode* addNode(const QString& nodeId) = 0;
    virtual CNode* getNode(const QString& nodeId, bool autoCreate = false) = 0;

	virtual CConnection* addEdge(const QString& edgeId, const QString& startNodeId, const QString& endNodeId) = 0;
	virtual CConnection* getEdge(const QString& edgeId) = 0;
	virtual bool setEdgeAttr(const QString& edgeId, const QByteArray& attrId, const QVariant& value) = 0;

    virtual QList<CConnection*> getEdges() const = 0;
    virtual QList<CNode*> getNodes() const = 0;
};


#endif // IGRAPHINTERFACE_H

#ifndef IGRAPHINTERFACE_H
#define IGRAPHINTERFACE_H

#include <QString>

class CConnection;
class CNode;


class IGraphInterface
{
public:
    virtual CConnection* addEdge(const QString& edgeId, const QString& startNodeId, const QString& endNodeId) = 0;
    virtual CNode* addNode(const QString& nodeId) = 0;
    virtual CNode* getNode(const QString& nodeId, bool autoCreate = false) = 0;

    virtual QList<CConnection*> getEdges() const = 0;
    virtual QList<CNode*> getNodes() const = 0;
};


#endif // IGRAPHINTERFACE_H

#ifndef CGRAPHINTERFACE_H
#define CGRAPHINTERFACE_H

#include "IGraphInterface.h"

class CNodeEditorScene;


class CGraphInterface: public IGraphInterface
{
public:
    CGraphInterface() {}
    explicit CGraphInterface(CNodeEditorScene& scene);
    virtual ~CGraphInterface() {}

    void setScene(CNodeEditorScene& scene) {
        m_scene = &scene;
    }

    // interface (to move out?)
    virtual CConnection* addEdge(const QString& edgeId, const QString& startNodeId, const QString& endNodeId);
    virtual CNode* addNode(const QString& nodeId);
    virtual CNode* getNode(const QString& nodeId, bool autoCreate = false);

    virtual QStringList getEdgeIds() const;
    virtual QStringList getNodeIds() const;

private:
    CNodeEditorScene *m_scene = nullptr;
};

#endif // CGRAPHINTERFACE_H
